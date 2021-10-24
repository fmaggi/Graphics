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
    free(registers.Components[TransformComponent_E]);
    free(registers.Components[SpriteComponent_E]);
    free(registers.Components[PhysicsComponent_E]);
    free(registers.used);
}

EntityID newEntity()
{
    static EntityID id = 0;
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

struct registryView ecs_view_egistry_internal(enum ComponentType type)
{
    struct registryView view;
    view.view = malloc(sizeof(EntityID) * count);
    view.count = 0;
    for (uint32_t i = 0; i < count; i++)
    {
        if (has_component_internal(i, type))
        {
            view.view[view.count++] = i;
        }
    }
    return view;
}


struct registryView ecs_group_view_internal(enum ComponentType t1, enum ComponentType t2)
{
    struct registryView view;
    view.view = malloc(sizeof(EntityID) * count);
    view.count = 0;
    for (uint32_t i = 0; i < count; i++)
    {
        if (has_component_internal(i, t1) && has_component_internal(i, t2))
        {
            view.view[view.count++] = i;
        }
    }
    return view;
}

void closeView(struct registryView view)
{
    free(view.view);
}

void* ecs_add_component_internal(EntityID id, enum ComponentType type, uint32_t size)
{
    assert(!has_component_internal(id, type));
    registers.used[id] |= ECS_TAG_VALUE(type);
    return ecs_get_component_internal(id, type, size);
}

void* ecs_get_component_internal(EntityID id, enum ComponentType type, uint32_t size)
{
    assert(has_component_internal(id, type));
    uint32_t offset = size * id;
    return (registers.Components[type] + offset);
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
    registers.Components[type] = temp;
}
