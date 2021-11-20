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
    {};
    inline EventType GetType() { return m_type; }
private:
    EventType m_type;
};

struct MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(float x, float y)
        : Event(EventType::MouseMoved), m_x(x), m_y(y)
    {};
    inline float GetX() { return m_x; }
    inline float GetY() { return m_y; }
private:
    float m_x, m_y;
};


#endif
