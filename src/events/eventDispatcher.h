#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include "event.h"
#include "log/log.h"

#include <functional>

#define BIND_EVENT_FN(fn) std::bind(fn, this, std::placeholders::_1)

template<typename T>
class EventHandler
{
public:
    using EventHandlerFn = std::function<bool (T /* event */)>;
    static void Dispatch(T event)
    {
        ASSERT(false, "Invalid event type dispatched!");
    }

    // Event Functions should only come from layers, as for now they are not removable
    static void RegisterOnEventFunction(EventHandlerFn onEvent)
    {
        ASSERT(false, "Invalid event function registered!");
    }
};

#endif
