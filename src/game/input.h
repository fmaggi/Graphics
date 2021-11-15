#ifndef INPUT_H
#define INPUT_H

#include "keys.h"
#include "glm/glm.hpp"

typedef int Keycode;
typedef int ButtonCode;

int isKeyPressed(Keycode key);
int isMouseButtonPressed(ButtonCode button);
glm::vec2 getCursorPos();

#endif
