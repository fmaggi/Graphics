#include "eventDispatcher.h"
#include "log/log.h"

#include "event.h"

#include <vector>
#include <algorithm>

#define INIT_HANDLER(type) \
    static std::vector<EventHandler<type>::EventHandlerFn> type##_handlers{}; \
    \
    template<> \
    void EventHandler<type>::Dispatch(type event) \
    { \
        for (auto it = type##_handlers.crbegin(); it != type##_handlers.crend(); ++it) \
        { \
            auto& f = *it; \
            if (f(event)) \
                return; \
        } \
    } \
    \
    template<> \
    void EventHandler<type>::RegisterOnEventFunction(EventHandlerFn onEvent) \
    { \
        type##_handlers.push_back(onEvent); \
    } \

INIT_HANDLER(WindowClose);
INIT_HANDLER(WindowResize);
INIT_HANDLER(KeyPressed);
INIT_HANDLER(KeyReleased);
INIT_HANDLER(MouseButtonPressed);
INIT_HANDLER(MouseButtonReleased);
INIT_HANDLER(MouseMoved);
INIT_HANDLER(MouseScrolled);

