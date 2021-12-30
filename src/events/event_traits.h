#ifndef EVENT_T_H
#define EVENT_T_H

#include <stdint.h>

struct event_index
{
    static uint32_t next()
    {
        static uint32_t currentIndex = 0;
        return currentIndex++;
    }
};

template<typename T>
struct EventIndex
{
    const static uint32_t value()
    {
        static const uint32_t value = event_index::next();
        return value;
    }
};

template<typename>
struct func_traits;

template<typename E>
struct func_traits<bool(*)(E)>
{
    using Event = E;
};

template<typename L, typename E>
struct func_traits<bool(L::*)(E)>
{
    using Event = E;
};

#endif
