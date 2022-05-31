// This file is part of the course TPV2@UCM - Samir Genaim
#pragma once

#include "../sockets/Serializable.h"
class Vector2D;

class infoMsg : public Serializable {  
public:
    static const size_t MESSAGE_SIZE = 4 * sizeof(float);

    infoMsg(){};
    infoMsg(Vector2D*a, Vector2D* b) : iniPos(a),newPos(b) {};
    ~infoMsg();
    void to_bin();
    int from_bin(char * bobj);

    Vector2D *iniPos;
    Vector2D *newPos;
};