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
        for (auto it = m_handlers_priority.rbegin(); it != m_handlers_priority.rend(); ++it)
        {
            const auto& handler = *it;
            if (handler.callback(event))
                return;
        }

        for (auto it = m_handlers.rbegin(); it != m_handlers.rend(); ++it)
        {
            const auto& handler = *it;
            if (handler.callback(event))
                return;
        }
    }

    template<typename L>
    static void RegisterListener(L* listener, MemberFunc<L> OnEvent)
    {
        auto callback = [listener, OnEvent](EventType event){
            return (listener->*OnEvent)(event);
        };

        m_handlers.emplace_back(callback, listener);
    }

    // functions which are not member functions are not removable. std::function does not support opertator== and I cannot find which function I want to remove
    static void RegisterListener(Func OnEvent)
    {
        m_handlers.emplace_back(OnEvent, nullptr);
    }

    static void RegisterListenerPriotity(Func OnEvent)
    {
        m_handlers_priority.emplace_back(OnEvent, nullptr);
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
    static inline std::vector<Listener> m_handlers_priority{};
};

#endif
