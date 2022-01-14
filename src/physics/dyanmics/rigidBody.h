#ifndef BODY_H
#define BODY_H

#include "glm/glm.hpp"

enum class BodyType;

struct AABB;

// Bodies should be created through the physics system. Not directly through this class
struct Body
{
    glm::vec2 translation;
    glm::vec2 velocity, force;
    float mass, Imass;

    uint32_t userFlags;
    void* userData;

    void (*onCollision)(Body* self, Body* other);
    BodyType type;
    AABB* aabb;
    Body* next = nullptr;
};

#endif
