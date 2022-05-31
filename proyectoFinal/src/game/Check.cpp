#include "Check.h"
#include "Board.h"

#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"

Check::Check(bool white, int x, int y, int oriX, int oriY){
    color = white;
    ori = new Vector2D(oriX,oriY);
    pos = new Vector2D(x,y);
    sdl = SDLUtils::instance();
    if(white){
        text = &sdl->images().at("checkWhite");
        textQueen =  &sdl->images().at("queenWhite");
    }
    else{
        text = &sdl->images().at("checkBlack");
        textQueen = &sdl->images().at("queenBlack");
    }
}

Check::~Check(){
    delete ori;
    ori = nullptr;
    delete pos;
    pos = nullptr;
    delete text;
    text = nullptr;
}

void Check::render(){
    Texture* act = text;
    if(queen)
        act = textQueen;
    SDL_Rect dest;
    dest.x = (pos->getX()*CELLSIZE + 3)*SCALE + ori->getX();
    dest.y = (pos->getY()*CELLSIZE + 3)*SCALE + ori->getY();
    dest.w = act->width()*SCALE;
    dest.h = act->width()*SCALE;
    act->render(dest);
}