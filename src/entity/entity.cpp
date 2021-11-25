#include "entity.h"

#include "registry.h"

#include "log/log.h"
#include "stdint.h"
#include "string.h"

#define INIT_COMPONENT(type) \
    TypeIndex<type>::value(); \
    registry.componentsList.push_back(new ComponentStorage<type>());

#define ECS_TAG_VALUE(x) (1 << x)

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

EntityID ECS::CreateEntity()
{
    EntityID index = registry.used.size();
    registry.used.push_back(0);
    return index;
}
