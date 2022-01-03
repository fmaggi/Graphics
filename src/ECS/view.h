#ifndef VIEW_H
#define VIEW_H

#include "registry.h"

#include "log/log.h"

#include <tuple>

template<typename... Ts>
class IteratorView
{
public:
    IteratorView(EntityID* base, EntityID* end, std::tuple<ComponentStorage<Ts>*...>* pools)
        : m_ptr(base), m_end(end), m_pools(pools)
    {
        while(!is_valid())
            ++m_ptr;
    }
    bool is_valid() const
    {
        return (m_ptr == m_end) || std::apply([e = *m_ptr](auto*... storage){ return (storage->Has(e) && ...); }, *m_pools);
    }

    IteratorView& operator++()
    {
        while(++m_ptr, !is_valid());
        return *this;
    }

    EntityID operator*()
    {
        return *m_ptr;
    }

    bool operator!=(IteratorView<Ts...> other)
    {
        return m_ptr != other.m_ptr;
    }

private:
    EntityID* m_ptr;
    EntityID* m_end;
    std::tuple<ComponentStorage<Ts>*...>* m_pools;
};

/**
 *  usage:
 *      auto view = ECS::View<SpriteComponent, TransformComponent>();
 *      for (EntityID e : view)
 *      {
 *          auto [sc, tc] = view.Get(e);
 *          // Do stuff
 *      }
 *
 */

template<typename T, typename... Ts>
class view
{
public:
    using Iterator = IteratorView<Ts...>;
public:
    view(ComponentStorage<T>* baseComponent, ComponentStorage<Ts>*... components)
        : m_baseComponent(baseComponent), m_pools{components...}
    {
        m_base = m_baseComponent->entities.data();
        m_size = m_baseComponent->entities.size();
    }

    std::tuple<T&, Ts&...> Get(EntityID id)
    {
        return std::tuple<T&, Ts&...>(m_baseComponent->Get(id), get_component_storage<Ts>()->Get(id)...);
    }

    Iterator begin()
    {
        return Iterator(m_base, m_base + m_size, &m_pools);
    }

    Iterator end()
    {
        return Iterator(m_base + m_size, m_base + m_size, &m_pools);
    }

    size_t size()
    {
        return m_baseComponent->entities.size();
    }

private:
    template<typename C>
    ComponentStorage<C>* get_component_storage()
    {
        ComponentStorage<C>* c = std::get<ComponentStorage<C>*>(m_pools);
        return c;
    }
private:
    ComponentStorage<T>* m_baseComponent;
    std::tuple<ComponentStorage<Ts>*...> m_pools;

    EntityID* m_base;
    size_t m_size;
};

// single-components view iterate directly over the components
// example:
//
//  auto view = ECS::View<TransformComponent>();
//  for (TransformComponent& tc : view)
//  {
//      /* Do stuff */
//  }

template<typename T>
class view<T>
{
public:
    using Iterator = typename std::vector<T>::iterator;
public:
    view(ComponentStorage<T>* baseComponent)
        : m_baseComponent(baseComponent)
    {}

    T& Get(auto id)
    {
        ASSERT(false, "Single-component views iterate over components directly. See view.h for an example!");
    }

    Iterator begin()
    {
        return m_baseComponent->components.begin();
    }

    Iterator end()
    {
        return m_baseComponent->components.end();
    }

    size_t size()
    {
        return m_baseComponent->entities.size();
    }

private:
    ComponentStorage<T>* m_baseComponent;
};

#endif
