#ifndef EVENT_H
#define EVENT_H

enum class EventType
{
    WindowClose = 0, WindowResize,
    KeyPressed, KeyReleased,
    MousePressed, MouseReleased, MouseMoved, MouseScrolled,
};

struct Event
{
public:
    Event(EventType type)
        : m_type(type)
    {}
    inline EventType GetType() const { return m_type; }
private:
    EventType m_type;
};

struct KeyEvent : public Event
{
    KeyEvent(int key_, int scancode_, int mods_, bool repeat_, EventType type)
        : Event(type), key(key_), scancode(scancode_), mods(mods_), repeat(repeat_)
    {}
    const int key, scancode, mods;
    const bool repeat;
};

struct MouseButtonEvent : public Event
{
    MouseButtonEvent(int button_, int mods_, EventType type)
        : Event(type), button(button_), mods(mods_)
    {}
    const int button, mods;
};

struct MouseMoved : public Event
{
    MouseMoved(int dx_, int dy_)
        : Event(EventType::MouseMoved), dx(dx_), dy(dy_)
    {}
    const float dx, dy;
};

struct MouseScrolled : public Event
{
    MouseScrolled(int dx_, int dy_)
        : Event(EventType::MouseScrolled), dx(dx_), dy(dy_)
    {}
    const float dx, dy;
};

struct WindowResize : public Event
{
    WindowResize(int w_, int h_)
        : Event(EventType::WindowResize), width(w_), height(h_)
    {}
    const float width, height;
};

struct WindowClose : public Event
{
    WindowClose()
        : Event(EventType::WindowClose)
    {}
};

#endif
