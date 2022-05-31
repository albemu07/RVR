#include "infoMsg.h"

#include <string.h>
#include "../utils/Vector2D.h"

void infoMsg::to_bin() {
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);
    char * _pointer = _data;

    float a = iniPos->getX();
    memcpy(_pointer, &a, sizeof(float));
    _pointer += sizeof(float);
    a = iniPos->getY();
    memcpy(_pointer, &a, sizeof(float));
    _pointer += sizeof(float);

    a = newPos->getX();
    memcpy(_pointer, &a, sizeof(float));
    _pointer += sizeof(float);
    a = newPos->getY();
    memcpy(_pointer, &a, sizeof(float));

}

int infoMsg::from_bin(char* bobj) {
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);
    char *  _pointer = _data;
    iniPos = new Vector2D();
    newPos = new Vector2D();

    float a;
    memcpy(&a, _pointer, sizeof(float));
    _pointer += sizeof(float);
    iniPos->setX(a);
    memcpy(&a, _pointer, sizeof(float));
    _pointer += sizeof(float);
    iniPos->setY(a);

    memcpy(&a, _pointer, sizeof(float));
    _pointer += sizeof(float);
    newPos->setX(a);
    memcpy(&a, _pointer, sizeof(float));
    newPos->setY(a);
    return 0;
}