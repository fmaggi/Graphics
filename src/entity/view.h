#ifndef VIEW_H
#define VIEW_H

#include "registry.h"

// this class is basically a wrapper over std::vector
// I thought of returning just a reference to the underlying vector
// but this allows me to be able to access the sparse set and hence the entity ids

template<typename T>
class view
{
public:
    using Iterator = typename std::vector<T>::iterator;
public:
    view(ComponentStorage<T>* componentStorage)
        : m_components(componentStorage) {}

    T& operator [] (size_t index) { return m_components->components[index]; }
    T  operator [] (size_t index) const { return m_components->components[index]; }

    size_t size() { return m_components->components.size(); }

    Iterator begin() { return m_components->components.begin(); }
    Iterator end()   { return m_components->components.end(); }

    Iterator cbegin() const { return m_components->components.cbegin(); }
    Iterator cend()   const { return m_components->components.cend(); }

    Iterator rbegin() { return m_components->components.rbegin(); }
    Iterator rend()   { return m_components->components.rend(); }
private:
    ComponentStorage<T>* m_components;
};

#endif
