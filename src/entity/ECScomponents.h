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

#include "glm/glm.hpp"

typedef struct
{
    EntityID id;
    glm::vec3 translation;
    float rotation;
    glm::vec2 scale;
} TransformComponent;

typedef struct
{
    EntityID id;
    glm::vec3 color;
    float texIndex;
} SpriteComponent;

typedef struct
{
    EntityID id;
    void* physicsBody;
} PhysicsComponent;

#endif
