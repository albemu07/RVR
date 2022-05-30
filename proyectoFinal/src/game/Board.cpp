#include "Board.h"
#include "../sdlutils/SDLUtils.h"

using namespace std;

Board::Board(){
    
}

Board::~Board(){

}

void Board::init(void){
    for(int i=0; i<12; i++){
        int x = i % 4 * 2 + 1;
        int y = i / 4;
        if(y == 1) x--;
        whites[i] = new Check(true, x, y);
        board[x][y].check = whites[i];

        x = i % 4 * 2;
        y = i / 4;
        if(y == 1) x++;
        blacks[i] = new Check(false, x, 7-y);
        board[x][7-y].check = blacks[i];
    }
    sdl = SDLUtils::instance();
    text = &sdl->images().at("board");
    nextM = &sdl->images().at("nextMove");
    lastM = &sdl->images().at("lastMove");
}

void Board::render(void){	
    text->render(0, 0);
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(board[i][j].check != nullptr)
                board[i][j].check->render();
            if(board[i][j].nextMove)
                nextM->render(i*32, j*32);
            if(board[i][j].marked)
                lastM->render(i*32, j*32);
        }
    }
}

void Board::handleInput(Vector2D pos){
    int x = pos.getX()/32;
    int y = pos.getY()/32;
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

void Board::markPossibleMoves(Vector2D pos){
    if(selectedCheck->isQueen()){
        cout << "Jugabilidad por programar\n";
    }
    else {
        int x = (int)pos.getX();
        int y = (int)pos.getY();
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
    board[x][y].check = c;
}