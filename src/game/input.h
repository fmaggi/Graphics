#ifndef INPUT_H
#define INPUT_H

#include "keys.h"

typedef int Keycode;
typedef int ButtonCode;

int isKeyPressed(Keycode key);
int isMouseButtonPressed(ButtonCode button);

#endif