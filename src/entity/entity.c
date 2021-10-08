#include "entity.h"

#include "register.h"
#include "assert.h"

#include "log/log.h"
#include "string.h"

#define DECL_COMPONENT(_name) \
    extern void _name##_init(); \
    extern void add##_name##Component(); \
    extern void* get##_name##Component();

#define DECL_COMPONENTS() \
    DECL_COMPONENT(Transform) \
    DECL_COMPONENT(Sprite) \

#define INIT_COMPONENT(_name) \
    _name##_init(maxEntities);

DECL_COMPONENTS()

#define ECS_TAG_VALUE(x) (1 << x)

static unsigned int getSize(enum ComponentType type);

static EntityID maxEntities = 16;
struct Register registers;

void initECS()
{
    INIT_COMPONENT(Transform);
    INIT_COMPONENT(Sprite);
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
    free(registers.transforms);
    free(registers.sprites);
    free(registers.used);
}

EntityID newEntity()
{
    static EntityID id = 0;
    return id++;
}

int hasComponent(EntityID id, enum ComponentType type)
{
    return registers.used[id] & ECS_TAG_VALUE(type);
}

void addComponent(EntityID id, enum ComponentType type, void* component)
{
    if (hasComponent(id, type))
        LOG_WARN("Entity %i already has component\n", id);
    registers.used[id] |= ECS_TAG_VALUE(type);
    unsigned int size = getSize(type);
    switch (type)
    {
        case transform: return addTransformComponent(id, *(TransformComponent*) component);
        case sprite:    return addSpriteComponent(id, *(SpriteComponent*) component);
    }
    LOG_WARN("Invalid component\n");
}

void* getComponent(EntityID id, enum ComponentType type)
{
    assert(hasComponent(id, type));
    switch (type)
    {
        case transform: return getTransformComponent(id);
        case sprite:    return getSpriteComponent(id);
    }

    LOG_WARN("Invalid component\n");
    return 0;
}

void* registerView(enum ComponentType type)
{
    switch (type)
    {
        case transform: return registers.transforms;
        case sprite:    return registers.sprites;
    }

    LOG_WARN("Invalid component\n");
    return 0;
}

static unsigned int getSize(enum ComponentType type)
{
    switch (type)
    {
        case transform: return sizeof(TransformComponent);
        case sprite:    return sizeof(SpriteComponent);
    }
}