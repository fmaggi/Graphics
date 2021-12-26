#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include "eventListener.h"

#include <vector>

template<typename EventType>
class EventSystem
{
private:
    using Listener = EventListener<EventType>;

    template<typename L>
    using MemberFunc = bool (L::*)(EventType);
    using Func = std::function<bool (EventType)>;
public:

    static void Emit(EventType event)
    {
        for (Listener handler : m_handlers)
        {
            if (handler.callback(event))
                return;
        }
    }

    template<typename L>
    static void RegisterListener(L* listener, MemberFunc<L> OnEvent)
    {
        auto callback = [=](EventType event){
            return (listener->*OnEvent)(event);
        };

        Listener l(callback, listener);
        m_handlers.push_back(l);
    }

    // functions which are not member functions are not removable. std::function does not support opertator== and I cannot find which function I want to remove
    static void RegisterFunction(Func OnEvent)
    {
        Listener l(OnEvent, nullptr);
        m_handlers.push_back(l);
    }

    template<typename L>
    static void UnregisterListener(L* listener)
    {
        if (!listener)
        {
            return;
        }
        void* handler_casted = static_cast<void*>(listener);
        auto it = std::find_if(m_handlers.begin(), m_handlers.end(), [h = handler_casted](Listener l){ return l.handler == h; });
        if (it != m_handlers.end())
            m_handlers.erase(it);
    }

private:
    static inline std::vector<Listener> m_handlers{};
};

#endif