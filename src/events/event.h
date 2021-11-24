#ifndef EVENT_H
#define EVENT_H

struct KeyPressed
{
    KeyPressed(int key_, int scancode_, int mods_, bool repeat_)
        : key(key_), scancode(scancode_), mods(mods_), repeat(repeat_)
    {}
    const int key, scancode, mods;
    const bool repeat;
};

struct KeyReleased
{
    KeyReleased(int key_, int scancode_, int mods_)
        : key(key_), scancode(scancode_), mods(mods_)
    {}
    const int key, scancode, mods;
};

struct MouseButtonPressed
{
    MouseButtonPressed(int button_, int mods_)
        : button(button_), mods(mods_)
    {}
    const int button, mods;
};

struct MouseButtonReleased
{
    MouseButtonReleased(int button_, int mods_)
        : button(button_), mods(mods_)
    {}
    const int button, mods;
};

struct MouseMoved
{
    MouseMoved(int dx_, int dy_)
        : dx(dx_), dy(dy_)
    {}
    const float dx, dy;
};

struct MouseScrolled
{
    MouseScrolled(int dx_, int dy_)
        : dx(dx_), dy(dy_)
    {}
    const float dx, dy;
};

struct WindowResize
{
    WindowResize(int w_, int h_)
        : width(w_), height(h_)
    {}
    const float width, height;
};

struct WindowClose
{
    WindowClose()
    {}
};

#endif
