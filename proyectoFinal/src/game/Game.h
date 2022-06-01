// This file is part of the course TPV2@UCM - Samir Genaim
#include "Board.h"

const uint32_t MAXTIME = 180000;

#pragma once

class InputHandler;
class SDLUtils;
class SDL_Renderer;
class Socket;

class Game {
public:
    Game(char* s, char* p, char* isServer);
    ~Game();
    void init(void);
    void gameLoop(void);
    void didIWin(void);
    void exitLoop(void);
private:
    void handleInput(void);
    void render(void);

    int creatingGame(void);
    int joinGame(void);
    int sendInfo(void); 
    int updateTime(void);
    int recvInfo(void);

    InputHandler* in;
    SDLUtils* sdl;
    SDL_Renderer* renderer;
    Board* b;
    bool exit_ = false;
    bool win = false;
    bool otherEnded = false;
    bool createGame = false;

    bool hasEaten = false;
    bool movedCheck = false;

    char* ips;
    char* port;

    Socket* sock = nullptr;
    Socket* other = nullptr;

    uint32_t myTime = MAXTIME;
    uint32_t enemyTime = MAXTIME;

    int so;
    int ot;
};


