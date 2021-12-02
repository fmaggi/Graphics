#include "entity.h"

#include "registry.h"

#include "log/log.h"
#include "stdint.h"
#include "string.h"

#include <algorithm>

#define INIT_COMPONENT(type) \
    TypeIndex<type>::value(); \
    registry.componentsList.push_back(new ComponentStorage<type>());

Registry ECS::registry;

void ECS::Init()
{
    INIT_COMPONENT(TransformComponent);
    INIT_COMPONENT(SpriteComponent);
    INIT_COMPONENT(PhysicsComponent);
}

void ECS::Destroy()
{
    for (auto c : registry.componentsList)
        free(c);
}

void ECS::DestroyEntity(EntityID id)
{
    for (int i = 0; i < registry.componentsList.size(); i++)
    {
        if (registry.used[id] & ECS_TAG_VALUE(i))
        {
            registry.componentsList[i]->DestroyComponent(id);
        }
    }

    registry.used[id] = 0;

    if (registry.freeEntities.size() < 100)
        registry.freeEntities.push_back(id);

    auto it = std::find(registry.aliveEntities.begin(), registry.aliveEntities.end(), id);
    ASSERT(it != registry.aliveEntities.end(), "Failed entity free");
    registry.aliveEntities.erase(it);
}

EntityID ECS::CreateEntity()
{
    EntityID entity;
    if (registry.freeEntities.size() > 0)
    {
        EntityID entity = registry.freeEntities[registry.freeEntities.size() - 1];
        registry.freeEntities.pop_back();
    }
    else
    {
        entity = registry.used.size();
        registry.used.push_back(0);
    }
    registry.aliveEntities.push_back(entity);
    return entity;
}

std::vector<EntityID>& ECS::AllEntities()
{
    return registry.aliveEntities;
}
