#ifndef ECS_H
#define ECS_H

#include "ECScomponents.h"
#include "registry.h"
#include "view.h"

#include "log/log.h"

#define ECS_TAG_VALUE(x) (1 << x)

class ECS
{
public:
    static void Init();
    static void Destroy();

    static EntityID CreateEntity();
    static void DestroyEntity(EntityID id);

    static std::vector<EntityID>& AllEntities();

    template<typename T>
    static bool HasComponent(EntityID id)
    {
        const uint32_t index = TypeIndex<T>::value();
        return registry.used[id] & ECS_TAG_VALUE(index);
    }

    template<typename T>
    static T& GetComponent(EntityID id)
    {
        ComponentStorage<T>& s = *GetComponentStorage<T>();
        return s.Get(id);
    }

    template<typename T>
    static T& AddComponent(EntityID id)
    {

        ComponentStorage<T>& s = *GetComponentStorage<T>();

        uint32_t componentIndex = TypeIndex<T>::value();

        registry.used[id] |= ECS_TAG_VALUE(componentIndex);

        uint32_t packedSetID = s.components.size();
        if (s.sparse.size() <= packedSetID)
            s.sparse.resize(packedSetID + 10);

        s.sparse[id] = packedSetID + 1;

        s.components.push_back({});
        s.entities.push_back(id);
        T& component = s.components[packedSetID];

        return component;
    }

    template<typename T, typename... Ts>
    static view<T, Ts...> View()
    {
        return view<T, Ts...>(GetComponentStorage<T>(), GetComponentStorage<Ts>()...);
    }

private:
    static Registry registry;

    template<typename T>
    static ComponentStorage<T>* GetComponentStorage()
    {
        const uint32_t componentIndex = TypeIndex<T>::value();
        ASSERT(componentIndex < registry.componentsList.size() && componentIndex < MAX_COMPONENT, "Invalid Component View");

        basic_component* b = registry.componentsList[componentIndex];
        ComponentStorage<T>* s = static_cast<ComponentStorage<T>*>(b);
        return s;
    }
};

#endif
