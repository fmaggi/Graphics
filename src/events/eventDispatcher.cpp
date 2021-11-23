#include "eventDispatcher.h"
#include "log/log.h"

#include <vector>

template<typename T>
class EventHandlerInternal
{
public:
    EventHandlerInternal() {}
    using EventHandlerFn = bool (*)(T& event);
    void DispatchInternal(Event& event)
    {
        T& e = static_cast<T&>(event);
        for (auto it = m_handlers.rbegin(); it != m_handlers.rend(); ++it)
        {
            auto& f = *it;
            if (f(e))
                return;
        }
    }

    void RegisterOnEventFunctionInternal(EventHandlerFn onEvent)
    {
        m_handlers.push_back(onEvent);
    }
private:
    std::vector<EventHandlerFn> m_handlers;
};

#define INIT_HANDLER(type) \
    static EventHandlerInternal<type> type##_handler; \
    \
    template<> \
    void EventHandler<type>::Dispatch(Event& event) \
    { \
        return type##_handler.DispatchInternal(event); \
    } \
    \
    template<> \
    void EventHandler<type>::RegisterOnEventFunction(EventHandlerFn onEvent) \
    { \
        return type##_handler.RegisterOnEventFunctionInternal(onEvent); \
    }

INIT_HANDLER(WindowClose);
INIT_HANDLER(WindowResize);
INIT_HANDLER(KeyPressed);
INIT_HANDLER(KeyReleased);
INIT_HANDLER(MouseButtonPressed);
INIT_HANDLER(MouseButtonReleased);
INIT_HANDLER(MouseMoved);
INIT_HANDLER(MouseScrolled);

