#ifndef INPUT_H
#define INPUT_H

#include "keys.h"
#include "glm/glm.hpp"

class Input
{
public:
    static bool isKeyPressed(Keycode key);
    static bool isMouseButtonPressed(ButtonCode button);
    static glm::vec2 getCursorPosition();
};


#endif
