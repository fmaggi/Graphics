#include "input.h"

#include "graphics/window.h"

namespace Input {

bool IsKeyPressed(Keycode key)
{
    return Window::IsKeyPressed(key);
}

bool IsMouseButtonPressed(ButtonCode button)
{
    return Window::IsMouseButtonPressed(button);
}

glm::vec2 GetCursorPosition()
{
    double x, y;
    Window::GetCursorPos(&x, &y);
    return {x, y};
}

};
