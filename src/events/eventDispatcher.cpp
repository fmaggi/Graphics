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
        for (auto it = type##_handlers.rbegin(); it != type##_handlers.rend(); ++it) \
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
    template<> \
    void EventHandler<type>::RemoveOnEventFunction(EventHandlerFn onEvent) \
    { \
        auto it = std::find(type##_handlers.begin(), type##_handlers.end(), onEvent); \
        if (it != type##_handlers.end()) \
            type##_handlers.erase(it); \
    }

INIT_HANDLER(WindowClose);
INIT_HANDLER(WindowResize);
INIT_HANDLER(KeyPressed);
INIT_HANDLER(KeyReleased);
INIT_HANDLER(MouseButtonPressed);
INIT_HANDLER(MouseButtonReleased);
INIT_HANDLER(MouseMoved);
INIT_HANDLER(MouseScrolled);

