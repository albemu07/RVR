#include "infoMsg.h"

#include <string.h>
#include "../utils/Vector2D.h"

infoMsg::~infoMsg() {
}

void infoMsg::to_bin() {
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);
    char * _pointer = _data;

    memcpy(_pointer, &type, sizeof(uint8_t));
    _pointer += sizeof(uint8_t);
    memcpy(_pointer, &timeLeft, sizeof(uint32_t));
    _pointer += sizeof(uint32_t);
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
    _pointer += sizeof(float);
    memcpy(_pointer, &turn, sizeof(bool));
}

int infoMsg::from_bin(char* bobj) {
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);
    char *  _pointer = _data;
    iniPos = new Vector2D();
    newPos = new Vector2D();

    memcpy(&type, _pointer, sizeof(uint8_t));
    _pointer += sizeof(uint8_t);
    memcpy(&timeLeft, _pointer, sizeof(uint32_t));
    _pointer += sizeof(uint32_t);
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
    _pointer += sizeof(float);
    newPos->setY(a);

    memcpy(&turn, _pointer, sizeof(bool));
    return 0;
}