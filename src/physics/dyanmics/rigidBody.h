#ifndef BODY_H
#define BODY_H

#include "glm/glm.hpp"

enum class BodyType;

struct AABB;

// Bodies should be created through the physics system. Not directly through this class
struct Body
{
    glm::vec3 translation;
    glm::vec2 velocity, impulse;

    uint32_t userFlags;
    void* userData;

    void (*onCollision)(Body* self, Body* other);
    BodyType type;
    AABB* aabb;
};

#endif
