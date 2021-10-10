#include "entity.h"

#include "register.h"
#include "assert.h"

#include "log/log.h"
#include "string.h"


#define INIT_COMPONENT(type) init_component_internal((type), sizeof(type##Component), maxEntities)

#define ECS_TAG_VALUE(x) (1 << x)

void init_component_internal(enum ComponentType type, unsigned int size, unsigned int count);

static unsigned int count = 0;
static EntityID maxEntities = 16;
struct Register registers;

void initECS()
{
    INIT_COMPONENT(Transform);
    INIT_COMPONENT(Sprite);
    INIT_COMPONENT(Physics);
    registers.used = malloc(sizeof(ComponentsUsed) * maxEntities);
    if (registers.used == NULL)
    {
        LOG_ERROR("Memory allocation error\n");
        exit(-1);
    }
    memset(registers.used, 0, sizeof(registers.used));
}

void destroyECS()
{
    free(registers.Components[Transform]);
    free(registers.Components[Sprite]);
    free(registers.used);
}

EntityID newEntity()
{
    static EntityID id = 0;
    registers.used[id] = 0;
    count++;
    return id++;
}

unsigned int getEntityCount()
{
    return count;
}

int hasComponent(EntityID id, enum ComponentType type)
{
    return registers.used[id] & ECS_TAG_VALUE(type);
}

void* registerView(enum ComponentType type)
{
    return registers.Components[type];
}

void* ecs_add_component_internal(EntityID id, enum ComponentType type, unsigned int size)
{
    assert(!hasComponent(id, type));
    registers.used[id] |= ECS_TAG_VALUE(type);
    return ecs_get_component_internal(id, type, size);
}

void* ecs_get_component_internal(EntityID id, enum ComponentType type, unsigned int size)
{
    assert(hasComponent(id, type));
    unsigned int offset = size * id;
    return (registers.Components[type] + offset);
}


void init_component_internal(enum ComponentType type, unsigned int size, unsigned int count)
{
    void* temp = malloc(size * count);
    if (temp == NULL)
    {
        LOG_ERROR("Memory allocation error\n");
        exit(-1);
    }
    memset(temp, 0, size * count);
    registers.Components[type] = temp;
}