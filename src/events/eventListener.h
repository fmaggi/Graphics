#ifndef EVENT_L_H
#define EVENT_L_H

#include <functional>

template<typename EventType>
struct EventListener
{
    using EventCallback = std::function<bool (EventType)>;

    EventListener(EventCallback callback_, void* handler_)
        : callback(callback_), handler(handler_)
    {}

    EventCallback callback;
    const void* handler;
};

#endif
