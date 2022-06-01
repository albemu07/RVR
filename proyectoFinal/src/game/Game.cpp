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
	delete ips;
    delete port;

    delete sock;
    delete other;

	delete in;
    delete sdl;
    delete b;
}

void Game::init()
{
	// Initialise the SDLGame singleton
	SDLUtils::init("SDLGame Demo!", 800, 600,
				   "resources/config/damas.resources.json");

	// reference to the SDLUtils Singleton. You could use it as a pointer as well,
	// I just prefer to use . instead of ->, it is just a matter of taste, nothing
	// else!
	//
	// you can also use the inline method sdlutils() that simply returns the value
	// of *SDLUtils::instance() --- it is defined at the end of SDLUtils.h
	//
	sdl = SDLUtils::instance();

	// show the cursor
	sdl->showCursor();

	// store the 'renderer' in a local variable, just for convenience
	renderer = sdl->renderer();

	// we can take textures from the predefined ones, and we can create a custom one as well
	// auto &sdlLogo = sdl->images().at("board");
	// auto &helloSDL = sdl.msgs().at("HelloSDL");
	// Texture pressAnyKey(renderer, "Press Esc key to exit",
	// 		sdl.fonts().at("ARIAL24"), build_sdlcolor(0x112233ff),
	// 		build_sdlcolor(0xffffffff));

	// start the music in a loop
	// sdl.musics().at("beat").play();

	// reference to the input handler (we could use a pointer, I just . rather than ->).
	// you can also use the inline method ih() that is defined in InputHandler.h
	in = InputHandler::instance();

	auto &iniImg = sdl->images().at("inicio");
	sdl->clearRenderer();
	iniImg.render(sdl->width()/2 - iniImg.width()/2*SCALE, sdl->height()/2 - iniImg.height()/2*SCALE);
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
	while (!exit_)
	{
		Uint32 startTime = sdl->currRealTime();

		handleInput();

		// sdl->clearRenderer();
		// sdlLogo.render(0, 0);
		// sdl->presentRenderer();
		render();

		if (movedCheck) {
			movedCheck = false;
			if (sendInfo() == -1)
				return;
		}
		if (!b->myTurn){
			if (recvInfo() == -1)
				return;
		}

		Uint32 frameTime = sdl->currRealTime() - startTime;

		if (frameTime < 20){
			SDL_Delay(20 - frameTime);
		}
		myTime -= 20;
		updateTime();
	}

	if (!otherEnded){
		infoMsg msg = infoMsg((int)msg.LOGOUT, 0, new Vector2D(0, 0), new Vector2D(0, 0), false);
		if (sock->send(msg) == -1){
			std::cout << "Error send: " << errno <<  "\n";
			close(so);
			if (createGame)
				close(ot);
			return;
		}
	}
	if (createGame){
		close(so);
		close(ot);
	} 
	// stop the music
	// Music::haltMusic();
}

void Game::exitLoop(){
	while(true){
		if (in->isKeyDown(SDLK_ESCAPE))
			exit_ = true;
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
	sdl->clearRenderer();
	if (win){
		auto &iniImg = sdl->images().at("ganar");
		iniImg.render(sdl->width()/2 - iniImg.width()/2*SCALE, sdl->height()/2 - iniImg.height()/2*SCALE);
	}
	else if (exit_){
		auto &iniImg = sdl->images().at("interrupción");
		iniImg.render(sdl->width()/2 - iniImg.width()/2*SCALE, sdl->height()/2 - iniImg.height()/2*SCALE);
	}
	else {
		auto &iniImg = sdl->images().at("perder");
		iniImg.render(sdl->width()/2 - iniImg.width()/2*SCALE, sdl->height()/2 - iniImg.height()/2*SCALE);
	}

	Texture pressAnyKey(renderer, "Press any key to exit",
	sdl->fonts().at("ARIAL24"), build_sdlcolor(0x112233ff),
	build_sdlcolor(0xffffffff));
	pressAnyKey.render((sdl->width() - pressAnyKey.width()) / 2, (sdl->height() - pressAnyKey.height()) / 2);
	sdl->presentRenderer();
	bool anykey = false;
	while (!anykey) {
		if (in->keyDownEvent())
			anykey = true;
	}
}

void Game::handleInput()
{

	in->clearState();
	in->refresh();

	// if (in->isKeyDown(SDLK_ESCAPE))
	// 	exit_ = true;
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
	std::cout << "Send bien\n";
	return 0;
}

int Game::recvInfo() {
	std::cout << "Waiting recv\n";
	infoMsg msg = infoMsg();
	if (sock->recv(msg) == -1) {
		std::cout << "Error recv: " << errno <<  "\n";
		close(so);
		if (createGame)
			close(ot);
		return -1;
	}
	std::cout << "Recv bien\n";
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
	return 0;
}