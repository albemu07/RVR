// This file is part of the course TPV2@UCM - Samir Genaim

#include <SDL2/SDL.h>

#include "Game.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"
#include "../sockets/Socket.h"

#include <unistd.h>

using namespace std;

Game::Game(char* s, char* p) {
	ips = s;
	port = p;
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
		myTurn = true;
	}
	else {
		if (joinGame() != 0)
		return;
	}
}

void Game::gameLoop() {
		// a boolean to exit the loop
	exit_ = false;
	b = new Board();
	b->init();
	while (!exit_)
	{
		Uint32 startTime = sdl->currRealTime();

		handleInput();

		update();
		refresh();

		// sdl->clearRenderer();
		// sdlLogo.render(0, 0);
		// sdl->presentRenderer();
		render();

		if (!myTurn) {
			sendInfo();
		}
		if (!enemyTurnOver){
			recvInfo();
		}

		Uint32 frameTime = sdl->currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}

	// stop the music
	// Music::haltMusic();
}

void Game::handleInput()
{

	in->clearState();
	in->refresh();

	if (in->isKeyDown(SDLK_ESCAPE))
		exit_ = true;
	if (in->mouseButtonDownEvent())
	{
		if (in->getMouseButtonState(0))
			b->handleInput(Vector2D(in->getMousePos().first, in->getMousePos().second));
	}
}

void Game::render()
{
	// clear screen
	sdl->clearRenderer();
	b->render();
	// render Hello SDL
	// helloSDL.render(x1, y1);
	// if (x1 + helloSDL.width() > winWidth)
	// 	helloSDL.render(x1 - winWidth, y1);
	// x1 = (x1 + 5) % winWidth;

	// render Press Any Key
	// pressAnyKey.render(x0, y0);

	// render the SDLogo
	// sdlLogo.render(x2, y2);

	// present new frame
	sdl->presentRenderer();
}

void Game::update()
{
}

void Game::refresh()
{
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

    printf("Conexión establecida");
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

	printf("Conexión establecida");
	return 0;
}

int Game::sendInfo() {
	// if (sock->send(info, *other) == -1){
	// 	std::cout << "Error send: " << errno <<  "\n";
	// 	close(so);
	// 	if (other)
	// 		close(ot);
	// 	return -1;
	// }
	return 0;
}

int Game::recvInfo() {
	// if (sock->recv(info) == -1) {
	// 	std::cout << "Error recv: " << errno <<  "\n";
	// 	close(so);
	// 	if (other)
	// 		close(ot);
	// 	return -1;
	// }
	return 0;
}