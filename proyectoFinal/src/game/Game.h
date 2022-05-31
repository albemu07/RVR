// This file is part of the course TPV2@UCM - Samir Genaim
#include "Board.h"


#pragma once

class InputHandler;
class SDLUtils;
class SDL_Renderer;
class Socket;

class Game {
public:
    Game(char* s, char* p);
    void init(void);
    void gameLoop(void);
private:
    void handleInput(void);
    void render(void);
    void update(void);
    void refresh(void);

    int creatingGame(void);
    int joinGame(void);
    int sendInfo(void); 
    int recvInfo(void);

    InputHandler* in;
    SDLUtils* sdl;
    SDL_Renderer* renderer;
    Board* b;
    bool exit_ = false;
    bool createGame = false;
    bool myTurn = false;
    bool enemyTurnOver = false;

    char* ips;
    char* port;

    Socket* sock = nullptr;
    Socket* other = nullptr;

    int so;
    int ot;
};


