#ifndef INTERFACE_EVENT_SYSTEM
#define INTERFACE_EVENT_SYSTEM

#include "basic_event_system.h"
#include "event.h"

#include "log/log.h"

#define TYPE_ASSERT_PASS(type) template<> constexpr bool assertType<type>() { return true; }

// Event system singleton interface
// I'm not sure about this. However, before I had a bunch of std::vectors allocated statically. Now I have a single pointer
// engine events are limited to those seven types.
// To support more event types you need to instantiate your own game event system
namespace EventSystem {

    namespace internal {
        extern basic_event_system* event_system;
    };

    template<typename E>
    constexpr bool assertType()
    {
        return false;
    }

    TYPE_ASSERT_PASS(WindowClose);
    TYPE_ASSERT_PASS(WindowResize);
    TYPE_ASSERT_PASS(KeyPressed);
    TYPE_ASSERT_PASS(KeyReleased);
    TYPE_ASSERT_PASS(MouseButtonPressed);
    TYPE_ASSERT_PASS(MouseButtonReleased);
    TYPE_ASSERT_PASS(MouseMoved);
    TYPE_ASSERT_PASS(MouseScrolled);

    template<typename E>
    void Emit(E event)
    {
        static_assert(assertType<E>(), "Invalid Event type!");
        internal::event_system->Emit(event);
    }

    template<auto OnEvent, typename L>
    void RegisterListener(L* listener)
    {
        using Event = typename func_traits<decltype(OnEvent)>::Event;
        static_assert(assertType<Event>(), "Invalid Event type!");

        internal::event_system->template RegisterListener<OnEvent, L>(listener);
    }

    template<auto OnEvent>
    void RegisterListener()
    {
        using Event = typename func_traits<decltype(OnEvent)>::Event;
        static_assert(assertType<Event>(), "Invalid Event type!");

        internal::event_system->template RegisterListener<OnEvent>();
    }

    template<auto OnEvent>
    void RegisterListenerPriotity()
    {
        using Event = typename func_traits<decltype(OnEvent)>::Event;
        static_assert(assertType<Event>(), "Invalid Event type!");

        internal::event_system->template RegisterListenerPriotity<OnEvent>();
    }

    template<auto OnEvent, typename L>
    void UnregisterListener(L* instance)
    {
        using Event = typename func_traits<decltype(OnEvent)>::Event;
        static_assert(assertType<Event>(), "Invalid Event type!");

        internal::event_system->template UnregisterListener<OnEvent, L>(instance);
    }

    template<auto OnEvent>
    void UnregisterListener()
    {
        using Event = typename func_traits<decltype(OnEvent)>::Event;
        static_assert(assertType<Event>(), "Invalid Event type!");

        internal::event_system->template UnregisterListener<OnEvent>();
    }

};

#endif
