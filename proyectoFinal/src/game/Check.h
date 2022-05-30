// This file is part of the course TPV2@UCM - Samir Genaim
#pragma once
#include "../utils/Vector2D.h"

class InputHandler;
class SDLUtils;
class SDL_Renderer;
class Texture;

class Check {  
public:
    Check(bool white, int x, int y);
    ~Check();
    void init(void);
    void handleInput(void);
    void render(void);
    void update(void);
    void refresh(void);
    inline bool isQueen(){return queen;};
    inline Vector2D* getPos(){return pos;};
    inline void setPos(int x, int y){pos = new Vector2D(x,y);};
    bool color;
private:
    bool queen = false;
    InputHandler* in;
    SDLUtils* sdl;   
    Texture* text;
    Vector2D* pos;
};