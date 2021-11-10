#ifndef INPUT_H
#define INPUT_H

#include "keys.h"

typedef int Keycode;
typedef int ButtonCode;

struct cursorPos
{
    double x, y;
};

int isKeyPressed(Keycode key);
int isMouseButtonPressed(ButtonCode button);
struct cursorPos getCursorPos();

#endif
