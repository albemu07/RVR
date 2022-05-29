// This file is part of the course TPV2@UCM - Samir Genaim
#include "Board.h"

#pragma once

class InputHandler;
class SDLUtils;
class SDL_Renderer;

class Game {
public:
    void init(void);
private:
    void handleInput(void);
    void render(void);
    void update(void);
    void refresh(void);

    InputHandler* in;
    SDLUtils* sdl;
    SDL_Renderer* renderer;
    Board* b;
    bool exit_ = false;
};


