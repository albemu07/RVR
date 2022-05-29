// This file is part of the course TPV2@UCM - Samir Genaim
#pragma once

#include "Check.h"

class InputHandler;
class SDLUtils;
class SDL_Renderer;
class Texture;

struct Cell{
    //bool pisada
    bool marked;
    //bool nextMove
    bool nextMove;
    //Ficha ficha
    Check check;
};

class Board {
    //Matriz casillas
    Cell board[8][8];
    
public:
    void init(void);
    void handleInput(void);
    void render(void);
    void update(void);
    void refresh(void);
private:
    
    InputHandler* in;
    SDLUtils* sdl;
    SDL_Renderer* renderer;
    Board* b;
    bool exit_ = false;
    Texture* text;
};