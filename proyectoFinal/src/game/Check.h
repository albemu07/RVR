// This file is part of the course TPV2@UCM - Samir Genaim
#pragma once
#include "../utils/Vector2D.h"

class InputHandler;
class SDLUtils;
class SDL_Renderer;
class Texture;

class Check {  
public:
    Check(bool white, int x, int y, int oriX, int oriY);
    ~Check();
    void init(void);
    void handleInput(void);
    void render(void);
    void update(void);
    void refresh(void);
    inline bool isQueen(){return queen;};
    inline void setQueen(bool q){queen = q;};
    inline Vector2D* getPos(){return pos;};
    inline void setPos(int x, int y){pos = new Vector2D(x,y);};
    bool color;
private:
    bool queen = false;
    InputHandler* in = nullptr;
    SDLUtils* sdl = nullptr;   
    Texture* text = nullptr;
    Texture* textQueen = nullptr;
    Vector2D* pos = nullptr;
    Vector2D* ori = nullptr;
};