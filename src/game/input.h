#ifndef INPUT_H
#define INPUT_H

#include "keys.h"
#include "glm/glm.hpp"

class Input
{
public:
    static bool IsKeyPressed(Keycode key);
    static bool IsMouseButtonPressed(ButtonCode button);
    static glm::vec2 GetCursorPosition();
};


#endif
