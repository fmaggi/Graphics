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

vec2s getCursorPos()
{
    double x, y;
    windowGetCursorPos(&x, &y);
    return (vec2s){{x, y}};
}
