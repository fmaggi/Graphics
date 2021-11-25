#ifndef ENTITY_H
#define ENTITY_H

#include "ECScomponents.h"
#include "registry.h"

typedef uint32_t EntityID;

#define ECS_TAG_VALUE(x) (1 << x)

class ECS
{
public:
    static void Init();
    static void Destroy();

    static EntityID CreateEntity();

    template<typename T>
    static bool HasComponent(EntityID id)
    {
        uint32_t index = TypeIndex<T>::value();
        return registry.used[id] & ECS_TAG_VALUE(index);
    }

    template<typename T>
    static T& GetComponent(EntityID id)
    {
        uint32_t componentIndex = TypeIndex<T>::value();
        basic_component* b = registry.componentsList[componentIndex];
        ComponentStorage<T>& s = *(static_cast<ComponentStorage<T>*>(b));
        ASSERT(componentIndex < registry.componentsList.size() && componentIndex < 8, "Invalid Component Get");
        uint32_t packedSetID = s.sparse[id];
        return s.components[packedSetID];
    }

    template<typename T>
    static T& AddComponent(EntityID id)
    {
        uint32_t componentIndex = TypeIndex<T>::value();
        basic_component* b = registry.componentsList[componentIndex];
        ComponentStorage<T>& s = *(static_cast<ComponentStorage<T>*>(b));
        ASSERT(componentIndex < registry.componentsList.size() && componentIndex < 8, "Invalid Component Add");
        registry.used[id] |= ECS_TAG_VALUE(componentIndex);

        uint32_t packedSetID = s.components.size();
        if (s.sparse.size() <= packedSetID)
            s.sparse.resize(packedSetID + 1);

        s.sparse[id] = packedSetID;

        s.components.push_back({});
        T& component = s.components[packedSetID];
        return component;
    }
private:
    static Registry registry;
};

#endif
