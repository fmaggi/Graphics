#ifndef VIEW_H
#define VIEW_H

#include "registry.h"

#include "log/log.h"

#include <tuple>

// this could probably be optimized
template<typename Fn, typename Tuple, size_t index>
static bool apply_impl(Fn func, Tuple& tuple)
{
    auto& element = std::get<index>(tuple);
    if constexpr (index)
        return func(element) && apply_impl<Fn, Tuple, index-1>(func, tuple);
    else
        return func(element);
}

template<typename Fn, typename Tuple>
static bool apply(Fn func, Tuple& tuple)
{
    constexpr size_t size = std::tuple_size<Tuple>{};
    return apply_impl<Fn, Tuple, size-1>(func, tuple);
}

template<typename... Ts>
class IteratorView
{
public:
    IteratorView(EntityID* base, EntityID* end, std::tuple<ComponentStorage<Ts>*...>* pools)
        : m_ptr(base), m_end(end), m_pools(pools) {}
    bool is_valid() const
    {
        return (m_ptr == m_end) || apply([e = *m_ptr](auto& storage){ return storage->has(e); }, *m_pools);
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

template<typename T, typename... Ts>
class view
{
public:
    using Iterator = IteratorView<Ts...>;
public:
    view(ComponentStorage<T>* baseComponent, ComponentStorage<Ts>*... components)
        : m_baseComponent(baseComponent), m_pools{components...}
    {
        base = m_baseComponent->entities.data();
        size = m_baseComponent->entities.size();
    }

    std::tuple<T&, Ts&...> Get(EntityID id)
    {
        return std::tuple<T&, Ts&...>(m_baseComponent->Get(id), get_component_storage<Ts>(id)...);
    }

    Iterator begin()
    {
        return Iterator(base, base + size, &m_pools);
    }

    Iterator end()
    {
        return Iterator(base + size, base + size, &m_pools);
    }

private:
    template<typename C>
    C& get_component_storage(EntityID id)
    {

        ComponentStorage<C>* c = std::get<ComponentStorage<C>*>(m_pools);
        return c->Get(id);
    }
private:
    std::tuple<ComponentStorage<Ts>*...> m_pools;
    ComponentStorage<T>* m_baseComponent;

    EntityID* base;
    size_t size;
};

#endif
