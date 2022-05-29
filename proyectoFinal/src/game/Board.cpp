#include "Board.h"
#include "../sdlutils/SDLUtils.h"

using namespace std;

void Board::init(void){
    sdl = SDLUtils::instance();
}

void Board::render(void){
    sdl->images().at("board");
}