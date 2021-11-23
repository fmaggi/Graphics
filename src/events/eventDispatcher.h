#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include "event.h"

#include "log/log.h"

template<typename T>
class EventHandler
{
public:
    using EventHandlerFn = bool (*)(T& event);
    static void Dispatch(Event& event)
    {
        ASSERT(false, "Invalid event type");
    }
    static void RegisterOnEventFunction(EventHandlerFn onEvent)
    {
        ASSERT(false, "Invalid event type");
    }
};

#endif
