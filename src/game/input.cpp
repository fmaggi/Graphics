#include "input.h"

#include "graphics/window.h"

int isKeyPressed(Keycode key)
{
    return windowIsKeyPressed(key);
}

int isMouseButtonPressed(ButtonCode button)
{
    return windowIsMouseButtonPressed(button);
}

glm::vec2 getCursorPos()
{
    double x, y;
    windowGetCursorPos(&x, &y);
    return {x, y};
}
