#include "event.h"

#include "graphics/gfx.h"

KeyEvent createKeyEvent(int key, int scancode, int action, int mods)
{
    KeyEvent e;
    e.key = key;
    e.scancode = scancode;
    e.action = action;
    e.mods = mods;
    e.repeat = action == GLFW_REPEAT ? 1 : 0;
    return e;
}