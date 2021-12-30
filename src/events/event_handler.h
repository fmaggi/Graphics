#ifndef EVENT_H_H
#define EVENT_H_H

#include <vector>

#include "eventListener.h"

#include "event_traits.h"

// this is just so that I can store templates together
class basic_event_handler {};

template<typename EventType>
class event_handler : public basic_event_handler
{
private:
    using Listener = EventListener<EventType>;
public:

    void Emit(EventType event)
    {
        for (auto it = m_listeners_priority.rbegin(); it != m_listeners_priority.rend(); ++it)
        {
            auto& listener = *it;
            if (listener(event))
                return;
        }

        for (auto it = m_listeners.rbegin(); it != m_listeners.rend(); ++it)
        {
            auto& listener = *it;
            if (listener(event))
                return;
        }
    }

    template<auto OnEvent, typename L>
    void RegisterListener(L* listener)
    {
        auto callback = wrapMember<OnEvent, L>;
        m_listeners.emplace_back(callback, listener);
    }

    template<auto OnEvent>
    void RegisterListener()
    {
        auto callback = wrap<OnEvent>;
        m_listeners.emplace_back(callback, nullptr);
    }

    template<auto OnEvent>
    void RegisterListenerPriotity()
    {
        auto callback = wrap<OnEvent>;
        m_listeners_priority.emplace_back(callback, nullptr);
    }

    template<auto OnEvent, typename L>
    void UnregisterListener(L* instance)
    {
        auto callback = wrapMember<OnEvent, L>;
        auto it = std::find_if(m_listeners.begin(), m_listeners.end(), [i = instance, c = callback](Listener l){ return l.handler == i && l.callback == c; });
        if (it != m_listeners.end())
            m_listeners.erase(it);
    }

    template<auto OnEvent>
    void UnregisterListener()
    {
        auto callback = wrap<OnEvent>;
        auto it = std::find_if(m_listeners.begin(), m_listeners.end(), [c = callback](Listener l){ return l.callback == c; });
        if (it != m_listeners.end())
            m_listeners.erase(it);
    }

private:
    template<auto Fn>
    static bool wrap(void* instance, EventType event)
    {
        return Fn(event);
    }

    template<auto Fn, typename L>
    static bool wrapMember(void* instance, EventType event)
    {
        L* l_instance = static_cast<L*>(instance);
        return (l_instance->*Fn)(event);
    }

    std::vector<Listener> m_listeners{};
    std::vector<Listener> m_listeners_priority{};
};

#endif
