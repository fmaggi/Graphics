#include "rigidBody.h"

#include "physics/collisions/aabb.h"

#include "log/log.h"

void addAABB(Body* body, float halfWidth, float halfHeight)
{
    if (body->aabbID != -1)
        LOG_WARN("Body already has an AABB\n");

    vec2s center = (vec2s){{body->position.x, body->position.y}};
    vec2s halfExtents = (vec2s){{halfWidth, halfHeight}};

    body->aabbID = createAABB2(center, halfExtents, body);
}
