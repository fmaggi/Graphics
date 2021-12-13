#ifndef INPUT_H
#define INPUT_H

#include "keys.h"
#include "glm/glm.hpp"

namespace Input {

    bool IsKeyPressed(Keycode key);
    bool IsMouseButtonPressed(ButtonCode button);
    glm::vec2 GetCursorPosition();

};


#endif
