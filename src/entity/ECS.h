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
        ComponentStorage<T>& s = *GetComponentStorage<T>();
        return s.Has(id);
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
        s.entities.push_back(id);

        uint32_t componentIndex = TypeIndex<T>::value();
        if (registry.used[id] & ECS_TAG_VALUE(componentIndex))
        {
            s.sparse[id].used = true;
            return s.Get(id);
        }

        registry.used[id] |= ECS_TAG_VALUE(componentIndex);

        return s.Add(id);
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
