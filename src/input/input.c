#include "input.h"

#include "graphics/window.h"

#include "log/log.h"

int isKeyPressed(Keycode key)
{
    return windowIsKeyPressed(key);
}
int isMouseButtonPressed(ButtonCode button)
{
    return windowIsMouseButtonPressed(button);
}