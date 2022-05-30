#include "Board.h"
#include "../sdlutils/SDLUtils.h"

using namespace std;

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
}

void Board::render(void){	
    text->render(0, 0);
    for(int i=0; i<12; i++){
        whites[i]->render();
        blacks[i]->render();
    }
}

void Board::handleInput(Vector2D pos){
    cout << "Hola buenas\n";
}