#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include "event.h"

#include <functional>

#define BIND_EVENT_FN(fn) std::bind(fn, this, std::placeholders::_1)

namespace EventSystem {

    void Dispatch(WindowClose event);
    void Dispatch(WindowResize event);
    void Dispatch(KeyPressed event);
    void Dispatch(KeyReleased event);
    void Dispatch(MouseButtonPressed event);
    void Dispatch(MouseButtonReleased event);
    void Dispatch(MouseMoved event);
    void Dispatch(MouseScrolled event);

    template<typename T>
    using EventCallback = std::function<bool (T)>;

    // TODO: make event callbacks removable
    void RegisterOnEventFunction(EventCallback<WindowClose> callback);
    void RegisterOnEventFunction(EventCallback<WindowResize> callback);
    void RegisterOnEventFunction(EventCallback<KeyPressed> callback);
    void RegisterOnEventFunction(EventCallback<KeyReleased> callback);
    void RegisterOnEventFunction(EventCallback<MouseButtonPressed> callback);
    void RegisterOnEventFunction(EventCallback<MouseButtonReleased> callback);
    void RegisterOnEventFunction(EventCallback<MouseMoved> callback);
    void RegisterOnEventFunction(EventCallback<MouseScrolled> callback);

}

#endif
