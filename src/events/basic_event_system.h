#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include "event_handler.h"

class basic_event_system
{
public:

    basic_event_system() = default;
    ~basic_event_system()
    {
        for (basic_event_handler* h : m_handlers)
            delete h;
    }

    template<typename E>
    void Emit(E event)
    {
        event_handler<E>* handler = GetHandler<E>();
        handler->Emit(event);
    }

    template<auto OnEvent, typename L>
    void RegisterListener(L* listener)
    {
        using E = func_traits<decltype(OnEvent)>::Event;

        event_handler<E>* handler = GetHandler<E>();
        handler->template RegisterListener<OnEvent, L>(listener);
    }

    template<auto OnEvent>
    void RegisterListener()
    {
        using E = func_traits<decltype(OnEvent)>::Event;

        event_handler<E>* handler = GetHandler<E>();
        handler->template RegisterListener<OnEvent>();
    }

    template<auto OnEvent>
    void RegisterListenerPriotity()
    {
        using E = func_traits<decltype(OnEvent)>::Event;
        event_handler<E>* handler = GetHandler<E>();
        handler->template RegisterListener<OnEvent>();
    }

    template<auto OnEvent, typename L>
    void UnregisterListener(L* instance)
    {
        using E = func_traits<decltype(OnEvent)>::Event;
        event_handler<E>* handler = GetHandler<E>();
        handler->template UnregisterListener<OnEvent>(instance);
    }

    template<auto OnEvent>
    void UnregisterListener()
    {
        using E = func_traits<decltype(OnEvent)>::Event;
        event_handler<E>* handler = GetHandler<E>();
        handler->template UnregisterListener<OnEvent>();
    }

private:
    template<typename E>
    event_handler<E>* GetHandler()
    {
        uint32_t index = EventIndex<E>::value();
        if (index >= m_handlers.size())
            m_handlers.push_back(new event_handler<E>);

        basic_event_handler* b = m_handlers[index];
        event_handler<E>* handlers = static_cast<event_handler<E>*>(b);
        return handlers;
    }

    std::vector<basic_event_handler*> m_handlers{};
};

#endif
