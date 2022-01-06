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
    virtual ~basic_component() = default;
    struct sparseObject
    {
        uint32_t packedSetID = 0;
        bool used = false;
    };

    std::vector<sparseObject> sparse{};
    std::vector<EntityID> entities{};

    bool Has(EntityID id)
    {
        if (id >= sparse.size())
            return false;
        return sparse[id].used;
    }

    void DestroyComponent(EntityID id)
    {
        uint32_t componentIndex = sparse[id].packedSetID;
        entities.erase(entities.begin() + componentIndex);
        sparse[id].used = false;
    }
};

template<typename T>
struct ComponentStorage : public basic_component
{
    std::vector<T> components{};

    T& Get(EntityID id)
    {
        uint32_t componentIndex = sparse[id].packedSetID;
        return components[componentIndex];
    }

    T& Add(EntityID id)
    {
        uint32_t packedSetID = components.size();
        if (sparse.size() <= packedSetID)
            sparse.resize(packedSetID + 10);

        sparse[id] = { packedSetID, true };

        components.push_back({});
        T& component = components[packedSetID];

        return component;
    }
};

struct Registry
{
    // recycled entities keep this flag set so we dont push new components to storages when recycled
    std::vector<ComponentsUsed> used;

    std::vector<basic_component*> componentsList;
    std::vector<EntityID> aliveEntities;
    std::vector<EntityID> freeEntities;
};

#endif
