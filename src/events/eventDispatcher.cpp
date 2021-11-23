#include "eventDispatcher.h"
#include "log/log.h"

#define INIT_HANDLER_VECTOR(type) \
    template<> \
    std::vector<EventHandler<type>::EventHandlerFn> EventHandler<type>::m_handlers{}

INIT_HANDLER_VECTOR(WindowClose);
INIT_HANDLER_VECTOR(WindowResize);
INIT_HANDLER_VECTOR(KeyEvent);
INIT_HANDLER_VECTOR(MouseButtonEvent);
INIT_HANDLER_VECTOR(MouseMoved);
INIT_HANDLER_VECTOR(MouseScrolled);
