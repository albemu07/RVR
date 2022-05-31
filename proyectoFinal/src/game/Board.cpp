#include "Board.h"
#include "../sdlutils/SDLUtils.h"

using namespace std;

Board::Board(){
    
}

Board::~Board(){
    delete pos;
    pos = nullptr;
}

void Board::init(void){
    sdl = SDLUtils::instance();
    text = &sdl->images().at("board");
    nextM = &sdl->images().at("nextMove");
    lastM = &sdl->images().at("lastMove");
    pos = new Vector2D(sdl->width()/2 - text->width()/2*SCALE, sdl->height()/2 - text->height()/2*SCALE);

    for(int i=0; i<12; i++){
        int x = i % 4 * 2 + 1;
        int y = i / 4;
        // if(y == 1) x--;
        // whites[i] = new Check(true, x, y, (int)pos->getX(), (int)pos->getY());
        // board[x][y].check = whites[i];

        x = i % 4 * 2;
        y = i / 4;
        if(y == 1) 
            x++;
        blacks[i] = new Check(false, x, 7-y, (int)pos->getX(), (int)pos->getY());
        board[x][7-y].check = blacks[i];
    }
}

void Board::render(void){	
    SDL_Rect dest;
    dest.x = pos->getX();
    dest.y = pos->getY();
    dest.w = text->width()*SCALE;
    dest.h = text->width()*SCALE;
    text->render(dest);
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            SDL_Rect dest;
            dest.w = nextM->width()*SCALE;
            dest.h = nextM->height()*SCALE;
            dest.x = i*CELLSIZE*SCALE + pos->getX();
            dest.y = j*CELLSIZE*SCALE + pos->getY();

            if(board[i][j].check != nullptr)
                board[i][j].check->render();
            if(board[i][j].nextMove)
                nextM->render(dest);
            if(board[i][j].marked)
                lastM->render(dest);            
        }
    }
}

void Board::handleInput(Vector2D p){
    int x = (p.getX()-pos->getX())/(CELLSIZE*SCALE);
    int y = (p.getY()-pos->getY())/(CELLSIZE*SCALE);
    if(board[x][y].check!=nullptr){
        selectedCheck = board[x][y].check;
        cleanNextMoves();
        markPossibleMoves(Vector2D(x,y));
    }
    else if(board[x][y].nextMove){
        cleanNextMoves();
        moveSelectedCheck(x, y);
        selectedCheck = nullptr;
    }
}

void Board::markPossibleMoves(Vector2D p){
    int x = (int)p.getX();
    int y = (int)p.getY();
    if(selectedCheck->isQueen()){
        bool continA = true, continB = true, continC = true, continD = true;
        for(int i=1; i<8 && (continA || continB || continC || continD); i++){
            if(continA && x-i >= 0 && y-i >= 0 && board[x-i][y-i].check == nullptr)
                board[x-i][y-i].nextMove = true;          
            else
                continA = false;
            if(continB && x+i < 8 && y-i >= 0 && board[x+i][y-i].check == nullptr)
                board[x+i][y-i].nextMove = true;
            else    
                continB = false;
            if(continC && x-i >= 0 && y+i < 8 && board[x-i][y+i].check == nullptr)
                board[x-i][y+i].nextMove = true;
            else
                continC = false;
            if(continD && x+i < 8 && y+i < 8 && board[x+i][y+i].check == nullptr)
                board[x+i][y+i].nextMove = true;
            else
                continD = false;
        }
    }
    else if(!selectedCheck->isQueen()){
        if(y-1 < 0) return;
        if(x-1>=0){
            if(board[x-1][y-1].check == nullptr)
                board[x-1][y-1].nextMove = true;
            else if(board[x-1][y-1].check->color != selectedCheck->color && x-2 >= 0 && board[x-2][y-2].check == nullptr)
                board[x-2][y-2].nextMove = true;
        }
        if(x+1<=7){
            if(board[x+1][y-1].check == nullptr)
                board[x+1][y-1].nextMove = true;
            else if(board[x+1][y-1].check->color != selectedCheck->color && x+2 <=7 && board[x+2][y-2].check == nullptr)
                board[x+2][y-2].nextMove = true;
        }
    }
}

void Board::cleanNextMoves(){
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            board[i][j].nextMove = false;
        }
    }
}

void Board::moveSelectedCheck(int x, int y){
    Vector2D* p = selectedCheck->getPos();
    Check* c = board[(int)p->getX()][(int)p->getY()].check;
    board[(int)p->getX()][(int)p->getY()].check = nullptr;
    c->setPos(x,y);
    if(y==0)
        c->setQueen(true);
    board[x][y].check = c;
}