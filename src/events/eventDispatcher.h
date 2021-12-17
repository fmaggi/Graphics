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

template<>
void EventHandler<WindowClose>::Dispatch(WindowClose event);
template<>
void EventHandler<WindowResize>::Dispatch(WindowResize event);
template<>
void EventHandler<KeyPressed>::Dispatch(KeyPressed event);
template<>
void EventHandler<KeyReleased>::Dispatch(KeyReleased event);
template<>
void EventHandler<MouseButtonPressed>::Dispatch(MouseButtonPressed event);
template<>
void EventHandler<MouseButtonReleased>::Dispatch(MouseButtonReleased event);
template<>
void EventHandler<MouseMoved>::Dispatch(MouseMoved event);
template<>
void EventHandler<MouseScrolled>::Dispatch(MouseScrolled event);

template<>
void EventHandler<WindowClose>::RegisterOnEventFunction(EventHandlerFn onEvent);
template<>
void EventHandler<WindowResize>::RegisterOnEventFunction(EventHandlerFn onEvent);
template<>
void EventHandler<KeyPressed>::RegisterOnEventFunction(EventHandlerFn onEvent);
template<>
void EventHandler<KeyReleased>::RegisterOnEventFunction(EventHandlerFn onEvent);
template<>
void EventHandler<MouseButtonPressed>::RegisterOnEventFunction(EventHandlerFn onEvent);
template<>
void EventHandler<MouseButtonReleased>::RegisterOnEventFunction(EventHandlerFn onEvent);
template<>
void EventHandler<MouseMoved>::RegisterOnEventFunction(EventHandlerFn onEvent);
template<>
void EventHandler<MouseScrolled>::RegisterOnEventFunction(EventHandlerFn onEvent);


#endif
