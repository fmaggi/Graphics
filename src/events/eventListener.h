#ifndef EVENT_L_H
#define EVENT_L_H

template<typename E>
struct EventListener
{
    using EventCallback = bool (*)(void*, E);

    EventListener(EventCallback callback_, void* handler_)
        : callback(callback_), handler(handler_)
    {}

    bool operator()(E event)
    {
        return callback(handler, event);
    }

    EventCallback callback;
    void* handler;
};

#endif
