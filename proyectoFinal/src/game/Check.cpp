#include "Check.h"

#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"

Check::Check(bool white, int x, int y){
    sdl = SDLUtils::instance();
    if(white){
        text = &sdl->images().at("checkWhite");
    }
    else{
        text = &sdl->images().at("checkBlack");
    }
    pos = new Vector2D(x,y);
}

Check::~Check(){

}

void Check::render(){
    text->render(pos->getX()*32 + 3, pos->getY()*32 + 3);
}