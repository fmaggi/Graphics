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

struct cursorPos getCursorPos()
{
    struct cursorPos pos;
    windowGetCursorPos(&pos.x, &pos.y);
    return pos;
}
