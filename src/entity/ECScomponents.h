#ifndef ECS_C_H
#define ECS_C_H

typedef int EntityID;

#define MAX_COMPONENT PhysicsComponent_E + 1
enum ComponentType
{
    TransformComponent_E = 0,
    SpriteComponent_E,
    PhysicsComponent_E,
};

#include "cglm/struct.h"

typedef struct
{
    EntityID id;
    vec3s position;
    float rotation;
    vec2s scale;
} TransformComponent;

typedef struct
{
    EntityID id;
    vec3s color;
    float texIndex;
} SpriteComponent;

typedef struct
{
    EntityID id;
    void* physicsBody;
} PhysicsComponent;

#endif
