#include "input.h"

#include "graphics/window.h"

bool Input::IsKeyPressed(Keycode key)
{
    return Window::IsKeyPressed(key);
}

bool Input::IsMouseButtonPressed(ButtonCode button)
{
    return Window::IsMouseButtonPressed(button);
}

glm::vec2 Input::GetCursorPosition()
{
    double x, y;
    Window::GetCursorPos(&x, &y);
    return {x, y};
}
