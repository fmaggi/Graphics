#ifndef REGISTER_H
#define REGISTER_H

#include <vector>
#include "stdint.h"

#include "log/log.h"

typedef uint8_t ComponentsUsed; // just a bit map to mark used components by entities. with uint8_t = 8 components per entity
typedef uint32_t EntityID;

struct index
{
    static uint32_t next()
    {
        static uint32_t currentIndex = 0;
        return currentIndex++;
    }
};

template<typename T>
struct TypeIndex
{
    static uint32_t value()
    {
        static const uint32_t value = index::next();
        return value;
    }
};

struct basic_component
{
    std::vector<EntityID> sparse{};
};

template<typename T>
struct ComponentStorage : public basic_component
{
    std::vector<T> components{};
};

struct Registry
{
    std::vector<ComponentsUsed> used;
    std::vector<basic_component*> componentsList;
};

#endif
