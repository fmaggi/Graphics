#ifndef INPUT_H
#define INPUT_H

#include "graphics/gfx.h"

struct Button
{
    int down, last, last_tick, pressed_tick;
};

struct Keyboard
{
    struct Button keys[GLFW_KEY_LAST];
};

struct Mouse
{
    struct Button buttons[GLFW_MOUSE_BUTTON_LAST];
};

#endif

