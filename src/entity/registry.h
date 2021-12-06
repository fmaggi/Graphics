#ifndef REGISTER_H
#define REGISTER_H

#include "entity.h"

#include <vector>
#include "stdint.h"

#include "log/log.h"

#define MAX_COMPONENT 8
typedef uint8_t ComponentsUsed; // just a bit map to mark used components by entities. with uint8_t = 8 components per entity

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
    const static uint32_t value()
    {
        static const uint32_t value = index::next();
        return value;
    }
};

struct basic_component
{
    std::vector<EntityID> sparse{};
    std::vector<EntityID> entities{};
    bool has(EntityID id)
    {
        if (id >= sparse.size())
            return false;
        return sparse[id] != 0;
    }
    virtual void DestroyComponent(EntityID id) = 0;
};

template<typename T>
struct ComponentStorage : public basic_component
{
    std::vector<T> components{};

    void DestroyComponent(EntityID id) override
    {
        uint32_t componentIndex = sparse[id] - 1;
        components.erase(components.begin() + componentIndex);
        entities.erase(entities.begin() + componentIndex);
        sparse.erase(sparse.begin() + id);
    }

    T& Get(EntityID id)
    {
        uint32_t componentIndex = sparse[id] - 1;
        return components[componentIndex];
    }
};

struct Registry
{
    std::vector<ComponentsUsed> used;
    std::vector<basic_component*> componentsList;
    std::vector<EntityID> aliveEntities;
    std::vector<EntityID> freeEntities;
};

#endif
