// This file is part of the course TPV2@UCM - Samir Genaim
#pragma once

#include "../sockets/Serializable.h"
#include <stdint.h>
class Vector2D;

class infoMsg : public Serializable {  
public:
    static const size_t MESSAGE_SIZE = 4 * sizeof(float) + sizeof(bool) + sizeof(uint8_t) + sizeof(uint32_t);

    enum msgType {
        LOGIN = 0,
        MOVEMENT = 1,
        LOGOUT = 2,
        TIME = 3,
        WIN = 4
    };

    infoMsg(){};
    infoMsg(uint8_t t, uint32_t time, Vector2D*a, Vector2D* b, bool c) : type(t), timeLeft(time),iniPos(a),newPos(b),turn(c) {};
    ~infoMsg();
    void to_bin();
    int from_bin(char * bobj);

    inline Vector2D * getIniPos() {
        return iniPos;
    }
    inline Vector2D * getNewPos() {
        return newPos;
    }
    inline bool getTurn() {
        return turn;
    }
    inline msgType getType(){
        return (msgType)type;
    }
    inline uint32_t getTime(){
        return timeLeft;
    }

private:
    uint8_t type;
    uint32_t timeLeft;
    Vector2D *iniPos;
    Vector2D *newPos;
    bool turn;
};