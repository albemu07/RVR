#include "Board.h"
#include "../sdlutils/SDLUtils.h"

using namespace std;

void Board::init(void){
    sdl = SDLUtils::instance();
    text = &sdl->images().at("board");
}
void Board::render(void){	
    text->render(0, 0);
}