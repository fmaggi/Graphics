#ifndef INPUT_H
#define INPUT_H

#include "keys.h"
#include "glm/glm.hpp"

class Input
{
public:
    static int isKeyPressed(Keycode key);
    static int isMouseButtonPressed(ButtonCode button);
    static glm::vec2 getCursorPos();
};


#endif
