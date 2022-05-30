// This file is part of the course TPV2@UCM - Samir Genaim
#pragma once

class InputHandler;
class SDLUtils;
class SDL_Renderer;
class Texture;
class Vector2D;

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
private:
    bool queen = false;
    InputHandler* in;
    SDLUtils* sdl;   
    Texture* text;
    Vector2D* pos;
};