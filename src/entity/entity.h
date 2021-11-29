#ifndef ENTITY_H
#define ENTITY_H

#include "ECScomponents.h"
#include "registry.h"
#include "view.h"

#include "log/log.h"

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
        const uint32_t index = TypeIndex<T>::value();
        return registry.used[id] & ECS_TAG_VALUE(index);
    }

    template<typename T>
    static T& GetComponent(EntityID id)
    {
        ASSERT(HasComponent<T>(id), "Entity doesn't have component");
        const uint32_t componentIndex = TypeIndex<T>::value();
        ASSERT(componentIndex < registry.componentsList.size() && componentIndex < 8, "Invalid Component Get");

        basic_component* b = registry.componentsList[componentIndex];
        ComponentStorage<T>& s = *(static_cast<ComponentStorage<T>*>(b));
        uint32_t packedSetID = s.sparse[id];
        return s.components[packedSetID];
    }

    template<typename T>
    static T& AddComponent(EntityID id)
    {
        const uint32_t componentIndex = TypeIndex<T>::value();
        ASSERT(componentIndex < registry.componentsList.size() && componentIndex < 8, "Invalid Component Add");

        basic_component* b = registry.componentsList[componentIndex];
        ComponentStorage<T>& s = *(static_cast<ComponentStorage<T>*>(b));
        registry.used[id] |= ECS_TAG_VALUE(componentIndex);

        uint32_t packedSetID = s.components.size();
        if (s.sparse.size() <= packedSetID)
            s.sparse.resize(packedSetID + 1);

        s.sparse[id] = packedSetID;

        s.components.push_back({});
        T& component = s.components[packedSetID];
        return component;
    }

    template<typename T>
    static view<T> View()
    {
        const uint32_t componentIndex = TypeIndex<T>::value();
        ASSERT(componentIndex < registry.componentsList.size() && componentIndex < 8, "Invalid Component View");

        basic_component* b = registry.componentsList[componentIndex];
        ComponentStorage<T>* s = static_cast<ComponentStorage<T>*>(b);
        return view<T>(s);
    }

private:
    static Registry registry;
};

#endif
