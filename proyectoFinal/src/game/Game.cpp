// This file is part of the course TPV2@UCM - Samir Genaim

#include <SDL2/SDL.h>

#include "Game.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"
#include "../sockets/Socket.h"
#include "infoMsg.h"

#include <unistd.h>

using namespace std;

Game::Game(char* s, char* p, char* isServer) {
	ips = s;
	port = p;
	if (isServer[0] == '1')
		createGame = true;
	else createGame = false;
	std::cout << createGame << "\n";
}

Game::~Game() {
	delete sock;
	if(createGame)
    	delete other;

    delete b;
}

void Game::init()
{
	// Initialise the SDLGame singleton
	SDLUtils::init("Chekers", 800, 600,
				   "resources/config/damas.resources.json");

	sdl = SDLUtils::instance();

	// show the cursor
	sdl->showCursor();

	// store the 'renderer' in a local variable, just for convenience
	renderer = sdl->renderer();

	in = InputHandler::instance();

	auto &iniImg = sdl->images().at("initMenu");
	sdl->clearRenderer();
	iniImg.render(0,0);
	sdl->presentRenderer();

	bool input = false;
	while (input){
		in->clearState();
		in->refresh();
		if (in->isKeyDown(SDLK_s)){
			createGame = true;
			input = true;
		}
		else if (in->isKeyDown(SDLK_c)){
			input = true;
		}
	}

	if (createGame){
		if (creatingGame() != 0)
			return;
	}
	else {
		if (joinGame() != 0)
			return;
	}
}

void Game::gameLoop() {
	// a boolean to exit the loop
	exit_ = false;
	b = new Board(createGame);
	b->init();
	while (!exit_ && !b->win && !b->lose)
	{
		Uint32 startTime = sdl->currRealTime();

		handleInput();

		render();

		if (movedCheck) {
			movedCheck = false;
			if (sendInfo() == -1)
				return;
		}

		Uint32 frameTime = sdl->currRealTime() - startTime;

		if (frameTime < 20){
			SDL_Delay(20 - frameTime);
		}
		if (b->myTurn) {
			myTime -= frameTime; 
			updateTime();
		}
	}

	if (!otherEnded && exit_){
		infoMsg msg = infoMsg((int)msg.LOGOUT, 0, new Vector2D(0, 0), new Vector2D(0, 0), false);
		if (sock->send(msg) == -1){
			std::cout << "Error send: " << errno <<  "\n";
			close(so);
			if (createGame)
				close(ot);
			return;
		}
	}

	if (b->win) {
		infoMsg msg = infoMsg((int)msg.WIN, 0, new Vector2D(0, 0), new Vector2D(0, 0), false);
		if (sock->send(msg) == -1){
			std::cout << "Error send: " << errno <<  "\n";
			close(so);
			if (createGame)
				close(ot);
			return;
		}
	}
}

void Game::exitLoop(){
	while(!exit_){
		recvInfo();
	}
}

int Game::updateTime() {
		infoMsg msg = infoMsg((int)msg.TIME, myTime, new Vector2D(0, 0), new Vector2D(0, 0), false);
		if (sock->send(msg) == -1){
			std::cout << "Error send: " << errno <<  "\n";
			close(so);
			if (createGame)
				close(ot);
			return -1;
		}
		return 0;
}

void Game::didIWin() {
	close(so);
	if (createGame){
		close(ot);
	} 

	sdl->clearRenderer();
	if (b->win){
		auto &iniImg = sdl->images().at("winMenu");
		iniImg.render(0,0);
	}
	else if (b->lose){
		auto &iniImg = sdl->images().at("loseMenu");
		iniImg.render(0,0);
	}
	else {
		auto &iniImg = sdl->images().at("interruptedMenu");
		iniImg.render(0,0);
	}

	Texture pressAnyKey(renderer, "Press any key to exit",
	sdl->fonts().at("ARIAL24"), build_sdlcolor(0x112233ff),
	build_sdlcolor(0xa17d6c00));
	pressAnyKey.render((sdl->width() - pressAnyKey.width()) / 2, 2 * sdl->height() / 3 - pressAnyKey.height() / 2);
	sdl->presentRenderer();
	bool anykey = false;
	while (!anykey) {
		in->clearState();
		in->refresh();
		if (in->keyDownEvent())
			anykey = true;
	}
}

void Game::handleInput()
{

	in->clearState();
	in->refresh();

	if (in->isKeyDown(SDLK_ESCAPE))
		exit_ = true;
	if (in->mouseButtonDownEvent() && b->myTurn)
	{
		if (in->getMouseButtonState(0))
			movedCheck = b->handleInput(Vector2D(in->getMousePos().first, in->getMousePos().second));
	}
}

void Game::render()
{
	// clear screen
	sdl->clearRenderer();
	b->render();
	
	Texture timer(renderer, to_string((int)enemyTime/1000),
	sdl->fonts().at("ARIAL24"), build_sdlcolor(0xffffffff),
	build_sdlcolor(0x112233ff));
	timer.render((sdl->width() - timer.width()) / 8, (sdl->height()/8 - timer.height()) / 8);

	Texture timer2(renderer, to_string((int)myTime/1000),
	sdl->fonts().at("ARIAL24"), build_sdlcolor(0xffffffff),
	build_sdlcolor(0x112233ff));
	timer2.render(7*(sdl->width() - timer2.width()) / 8, 7*(sdl->height() - timer2.height()) / 8);

	sdl->presentRenderer();
}

int Game::creatingGame()
{
	sock = new Socket(ips, port);
	so = sock->socketDesc();
	if(sock->bind() != 0){
		std::cout << "Error bind: " << errno <<  "\n";
        close(so);
        return -1;
	}

	if(sock->listen() != 0){
		std::cout << "Error listen: " << errno <<  "\n";
        close(so);
        return -1;
	}

	ot = sock->accept();
	if (ot == -1) {
		std::cout << "Error accept: " << errno <<  "\n";
        close(so);
        close(ot);
        return -1;
	}

	infoMsg msg = infoMsg();
	if (sock->recv(msg) == -1) {
		std::cout << "Error recv: " << errno <<  "\n";
		close(so);
		close(ot);
		return -1;
	}
	printf("Conexión establecida\n");
	return 0;
}

int Game::joinGame()
{
	sock = new Socket(ips, port);
	so = sock->socketDesc();

	if (sock->connect() != 0){
        std::cout << "Error connect: " << errno <<  "\n";
        close(so);
        return -1;
	}

	other = sock;
	ot = so;
	infoMsg msg = infoMsg((int)msg.LOGIN, 0, new Vector2D(0, 0), new Vector2D(0, 0), false);
	if (sock->send(msg) == -1){
		std::cout << "Error send: " << errno <<  "\n";
		close(so);
		return -1;
	}
	printf("Conexión establecida\n");

	return 0;
}

int Game::sendInfo() {
	infoMsg msg = infoMsg((uint8_t)msg.MOVEMENT, 0, b->getOldPos(), b->getNewPos(), b->myTurn);
	if (sock->send(msg) == -1){
		std::cout << "Error send: " << errno <<  "\n";
		close(so);
		if (createGame)	
			close(ot);
		return -1;
	}
	return 0;
}

int Game::recvInfo() {
	infoMsg msg = infoMsg();
	if (sock->recv(msg) == -1) {
		std::cout << "Error recv: " << errno <<  "\n";
		close(so);
		if (createGame)
			close(ot);
		return -1;
	}
	if (msg.getType() == msg.MOVEMENT) {
		b->processMovement(msg.getIniPos(), msg.getNewPos());
		b->myTurn= !msg.getTurn();
	}
	else if (msg.getType() == msg.LOGOUT){
		exit_ = true;
		otherEnded = true;
	}
	else if (msg.getType() == msg.TIME){
		enemyTime = msg.getTime();
	}
	else if (msg.getType() == msg.WIN){
		b->lose = true;
	}
	return 0;
}