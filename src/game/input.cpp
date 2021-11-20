#include "input.h"

#include "graphics/window.h"

int Input::isKeyPressed(Keycode key)
{
    return Window::IsKeyPressed(key);
}

int Input::isMouseButtonPressed(ButtonCode button)
{
    return Window::IsMouseButtonPressed(button);
}

glm::vec2 Input::getCursorPos()
{
    double x, y;
    Window::GetCursorPos(&x, &y);
    return {x, y};
}
