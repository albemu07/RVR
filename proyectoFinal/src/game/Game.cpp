// This file is part of the course TPV2@UCM - Samir Genaim

#include <SDL2/SDL.h>

#include "Game.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"

using namespace std;

void Game::init() {

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

	//show the cursor
	sdl->showCursor();

	// store the 'renderer' in a local variable, just for convenience
	renderer = sdl->renderer();

	// we can take textures from the predefined ones, and we can create a custom one as well
	// auto &sdlLogo = sdl.images().at("sdl_logo");
	// auto &helloSDL = sdl.msgs().at("HelloSDL");
	// Texture pressAnyKey(renderer, "Press Esc key to exit",
	// 		sdl.fonts().at("ARIAL24"), build_sdlcolor(0x112233ff),
	// 		build_sdlcolor(0xffffffff));

	// some coordinates
	// auto x0 = (winWidth - pressAnyKey.width()) / 2;
	// auto y0 = (winHeight - pressAnyKey.height()) / 2;
	// auto x1 = 0;
	// auto y1 = y0 - 4 * pressAnyKey.height();
	// auto x2 = (winWidth - sdlLogo.width()) / 2;
	// auto y2 = y0 + 2 * pressAnyKey.height();

	// start the music in a loop
	//sdl.musics().at("beat").play();

	// reference to the input handler (we could use a pointer, I just . rather than ->).
	// you can also use the inline method ih() that is defined in InputHandler.h
	in = InputHandler::instance();

	// a boolean to exit the loop
	exit_ = false;

	while (!exit_) {
		Uint32 startTime = sdl->currRealTime();

		handleInput();

		render();

		Uint32 frameTime = sdl->currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}

	// stop the music
	//Music::haltMusic();

}

void Game::handleInput() {

	in->clearState();
	in->refresh();
	
	if (in->isKeyDown(SDLK_ESCAPE))
		exit_ = true;
}

void Game::render() {
	// clear screen
	sdl->clearRenderer();

	// render Hello SDL
	// helloSDL.render(x1, y1);
	// if (x1 + helloSDL.width() > winWidth)
	// 	helloSDL.render(x1 - winWidth, y1);
	// x1 = (x1 + 5) % winWidth;

	// render Press Any Key
	//pressAnyKey.render(x0, y0);

	// render the SDLogo
	//sdlLogo.render(x2, y2);

	// present new frame
	sdl->presentRenderer();
}