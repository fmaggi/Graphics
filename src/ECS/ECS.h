#ifndef ECS_H
#define ECS_H

#include "ECScomponents.h"
#include "registry.h"
#include "view.h"

#include "log/log.h"

#define ECS_TAG_VALUE(x) (1 << x)

class ECS {
public:
    ECS() = default;
    ~ECS();

    EntityID CreateEntity();
    void DestroyEntity(EntityID id);

    std::vector<EntityID>& AllEntities();

    template<typename T>
    bool HasComponent(EntityID id)
    {
        ComponentStorage<T>& s = *GetComponentStorage<T>();
        return s.Has(id);
    }

    template<typename T>
    T& GetComponent(EntityID id)
    {
        ComponentStorage<T>& s = *GetComponentStorage<T>();
        return s.Get(id);
    }

    template<typename T>
    T& AddComponent(EntityID id)
    {
        ComponentStorage<T>& s = *GetComponentStorage<T>();
        s.entities.push_back(id);

        uint32_t componentIndex = TypeIndex<T>::value();
        if (m_registry.used[id] & ECS_TAG_VALUE(componentIndex))
        {
            s.sparse[id].used = true;
            return s.Get(id);
        }

        // this flag is used when recycling entities
        m_registry.used[id] |= ECS_TAG_VALUE(componentIndex);

        return s.Add(id);
    }

    template<typename T, typename... Ts>
    view<T, Ts...> View()
    {
        return view<T, Ts...>(GetComponentStorage<T>(), GetComponentStorage<Ts>()...);
    }

private:
    Registry m_registry;

    template<typename T>
    ComponentStorage<T>* GetComponentStorage()
    {
        const uint32_t componentIndex = TypeIndex<T>::value();
        if (componentIndex >= m_registry.componentsList.size()) {
            m_registry.componentsList.resize(componentIndex + 1);
            m_registry.componentsList[componentIndex] = new ComponentStorage<T>;
        }

        basic_component* b = m_registry.componentsList[componentIndex];
        ComponentStorage<T>* s = static_cast<ComponentStorage<T>*>(b);
        return s;
    }
};

#endif
