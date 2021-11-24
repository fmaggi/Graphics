#include "input.h"

#include "graphics/window.h"

bool Input::isKeyPressed(Keycode key)
{
    return Window::IsKeyPressed(key);
}

bool Input::isMouseButtonPressed(ButtonCode button)
{
    return Window::IsMouseButtonPressed(button);
}

glm::vec2 Input::getCursorPosition()
{
    double x, y;
    Window::GetCursorPos(&x, &y);
    return {x, y};
}
