#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include "log/log.h"

template<typename T>
class EventHandler
{
public:
    using EventHandlerFn = bool (*)(T event);
    static void Dispatch(T event)
    {
        ASSERT(false, "Invalid event type dispatched!");
    }
    static void RegisterOnEventFunction(EventHandlerFn onEvent)
    {
        ASSERT(false, "Invalid event function registered!");
    }
    static void RemoveOnEventFunction(EventHandlerFn onEvent)
    {
        ASSERT(false, "Invalid event function removed!");
    }
};

#endif
