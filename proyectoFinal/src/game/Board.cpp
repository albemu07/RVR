#include "Board.h"
#include "../sdlutils/SDLUtils.h"

using namespace std;

Board::~Board(){

}

void Board::init(void){
    for(int i=0; i<12; i++){
        whites[i] = new Check(true, 1, 3);
        board[1][3].check = whites[i];
        blacks[i] = new Check(false, 0, 0);
    }
    sdl = SDLUtils::instance();
    text = &sdl->images().at("board");
}

void Board::render(void){	
    text->render(0, 0);
    for(int i=0; i<12; i++){
        whites[i]->render();
        blacks[i]->render();
    }
}