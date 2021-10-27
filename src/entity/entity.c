#include "entity.h"

#include "register.h"
#include "assert.h"

#include "log/log.h"
#include "stdint.h"
#include "string.h"


#define INIT_COMPONENT(type) init_component_internal((type##_E), sizeof(type), maxEntities)

#define ECS_TAG_VALUE(x) (1 << x)

void init_component_internal(enum ComponentType type, uint32_t size, uint32_t count);

static uint32_t count = 0;
static EntityID maxEntities = 16;
struct Register registers;

void initECS()
{
    LOG_TRACE("Initializing ECS");
    INIT_COMPONENT(TransformComponent);
    INIT_COMPONENT(SpriteComponent);
    INIT_COMPONENT(PhysicsComponent);
    registers.used = malloc(sizeof(ComponentsUsed) * maxEntities);
    if (registers.used == NULL)
    {
        LOG_ERROR("Memory allocation error");
        exit(-1);
    }
    memset(registers.used, 0, sizeof(ComponentsUsed) * maxEntities);
}

void destroyECS()
{
    free(registers.Components[TransformComponent_E].components);
    free(registers.Components[SpriteComponent_E].components);
    free(registers.Components[PhysicsComponent_E].components);
    free(registers.used);
}

EntityID newEntity()
{
    static EntityID id = 0;
    registers.used[id] = 0;
    count++;
    return id++;
}

uint32_t getEntityCount()
{
    return count;
}

int has_component_internal(EntityID id, enum ComponentType type)
{
    return registers.used[id] & ECS_TAG_VALUE(type);
}

struct View ecs_view_internal(enum ComponentType type)
{
    struct View view;
    view.component = registers.Components[type].components;
    view.count = registers.Components[type].count;

    return view;
}

void* ecs_view_get_internal(struct View* view, uint32_t size, enum ComponentType type, uint32_t i)
{
    while (!(has_component_internal(i, type)))
        i++;

    return view->component + i*size;
}

struct Group ecs_group_view_internal(enum ComponentType t1, enum ComponentType t2)
{
    struct Group group;
    group.count = registers.Components[t1].count > registers.Components[t2].count ? registers.Components[t2].count : registers.Components[t1].count;
    group.mask = ECS_TAG_VALUE(t1) | ECS_TAG_VALUE(t2);
    group.component1 = registers.Components[t1].components;
    group.component2 = registers.Components[t2].components;
    return group;
}

struct GroupObject ecs_group_get_internal(struct Group* group, uint32_t size1, uint32_t size2, uint32_t i)
{
    while((registers.used[i] & group->mask) != group->mask)
        i++;

    struct GroupObject o;
    o.component1 = group->component1 + i*size1;
    o.component2 = group->component2 + i*size2;
    return o;
}

void* ecs_add_component_internal(EntityID id, enum ComponentType type, uint32_t size)
{
    ASSERT(!has_component_internal(id, type), "Entity already has component");
    registers.used[id] |= ECS_TAG_VALUE(type);
    registers.Components[type].count++;
    return ecs_get_component_internal(id, type, size);
}

void* ecs_get_component_internal(EntityID id, enum ComponentType type, uint32_t size)
{
    ASSERT(has_component_internal(id, type), "Entity doesn't have component");
    uint32_t offset = size * id;
    return (registers.Components[type].components + offset);
}

void init_component_internal(enum ComponentType type, uint32_t size, uint32_t count)
{
    void* temp = malloc(size * count);
    if (temp == NULL)
    {
        LOG_ERROR("Memory allocation error");
        exit(-1);
    }
    memset(temp, 0, size * count);
    registers.Components[type].count = 0;
    registers.Components[type].components = temp;
}
