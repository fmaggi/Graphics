#ifndef INTERFACE_EVENT_SYSTEM
#define INTERFACE_EVENT_SYSTEM

#include "basic_event_system.h"

// Event system singleton interface
// I'm not sure about this. However, before I had a bunch of std::vectors allocated statically. Now I have a single pointer
namespace EventSystem {

    namespace internal_e {
        extern basic_event_system* event_system;
    };

    template<typename E>
    void Emit(E event)
    {
        internal_e::event_system->Emit(event);
    }

    template<auto OnEvent, typename L>
    void RegisterListener(L* listener)
    {
        internal_e::event_system->template RegisterListener<OnEvent, L>(listener);
    }

    template<auto OnEvent>
    void RegisterListener()
    {
        internal_e::event_system->template RegisterListener<OnEvent>();
    }

    template<auto OnEvent>
    void RegisterListenerPriotity()
    {
        internal_e::event_system->template RegisterListenerPriotity<OnEvent>();
    }

    template<auto OnEvent, typename L>
    void UnregisterListener(L* instance)
    {
        internal_e::event_system->template UnregisterListener<OnEvent, L>(instance);
    }

    template<auto OnEvent>
    void UnregisterListener()
    {
        internal_e::event_system->template UnregisterListener<OnEvent>();
    }

};

#endif
