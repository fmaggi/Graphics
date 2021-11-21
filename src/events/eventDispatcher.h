#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include "event.h"
#include "log/log.h"

#include <vector>

template<typename T>
class EventHandler
{
public:
    using EventHandlerFn = bool (*)(T& event);
    static void Dispatch(Event& event)
    {
        T& e = static_cast<T&>(event);
        for (auto& f : m_handlers)
        {
            if (f(e))
                return;
        }
    }
    static void RegisterOnEventFunction(EventHandlerFn onEvent)
    {
        m_handlers.push_back(onEvent);
    }
private:
    static std::vector<EventHandlerFn> m_handlers;
};

#endif
