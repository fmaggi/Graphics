#include "eventDispatcher.h"
#include "log/log.h"

template<>
std::vector<EventHandler<WindowClose>::EventHandlerFn> EventHandler<WindowClose>::m_handlers{};

template<>
std::vector<EventHandler<WindowResize>::EventHandlerFn> EventHandler<WindowResize>::m_handlers{};

template<>
std::vector<EventHandler<KeyEvent>::EventHandlerFn> EventHandler<KeyEvent>::m_handlers{};

template<>
std::vector<EventHandler<MouseScrolled>::EventHandlerFn> EventHandler<MouseScrolled>::m_handlers{};

template<>
std::vector<EventHandler<MouseButtonEvent>::EventHandlerFn> EventHandler<MouseButtonEvent>::m_handlers{};

template<>
std::vector<EventHandler<MouseMoved>::EventHandlerFn> EventHandler<MouseMoved>::m_handlers{};
