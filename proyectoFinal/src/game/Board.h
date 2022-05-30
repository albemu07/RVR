// This file is part of the course TPV2@UCM - Samir Genaim
#pragma once

#include "Check.h"

class InputHandler;
class SDLUtils;
class SDL_Renderer;
class Texture;
class Vector2D;

struct Cell{
    //bool pisada
    bool marked;
    //bool nextMove
    bool nextMove;
    //Ficha ficha
    Check* check;
};

class Board {
    //Matriz casillas
    Cell board[8][8];
    Check* whites[12];
    Check* blacks[12];
public:
    Board(){};
    ~Board();
    void init(void);
    void handleInput(Vector2D pos);
    void render(void);
    void update(void);
    void refresh(void);
private:
    
    InputHandler* in;
    SDLUtils* sdl;
    Board* b;
    Texture* text;
};