#include "ECS.h"

#include "registry.h"

#include "log/log.h"
#include "stdint.h"
#include "string.h"

#include <algorithm>

#define INIT_COMPONENT(type) \
    TypeIndex<type>::value(); \
    m_registry.componentsList.push_back(new ComponentStorage<type>());

ECS::~ECS()
{
    for (auto c : m_registry.componentsList)
        delete c;
}

void ECS::DestroyEntity(EntityID id)
{
    for (int i = 0; i < m_registry.componentsList.size(); i++)
    {
        if (m_registry.used[id] & ECS_TAG_VALUE(i))
        {
            m_registry.componentsList[i]->DestroyComponent(id);
        }
    }

    m_registry.freeEntities.push_back(id);

    auto it = std::find(m_registry.aliveEntities.begin(), m_registry.aliveEntities.end(), id);
    ASSERT(it != m_registry.aliveEntities.end(), "Failed entity free");
    m_registry.aliveEntities.erase(it);
}

EntityID ECS::CreateEntity()
{
    EntityID entity;
    if (!m_registry.freeEntities.empty())
    {
        entity = m_registry.freeEntities[m_registry.freeEntities.size() - 1];
        m_registry.freeEntities.pop_back();
    }
    else
    {
        entity = m_registry.used.size();
        m_registry.used.push_back(0);
    }
    m_registry.aliveEntities.push_back(entity);
    return entity;
}

std::vector<EntityID>& ECS::AllEntities()
{
    return m_registry.aliveEntities;
}
