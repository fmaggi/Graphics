#include "rigidBody.h"

#include "collision/aabb.h"


void createBody(vec3s position, enum BodyType type, int32t flags)
{
    
}

void addAABB(Body* body, float halfWidth, float halfHeight)
{
    if (body->aabbID != -1)
        LOG_WARN("Body already has an AABB\n");

    vec2s min;
    vec2s max;

    min.x = body->position.x - halfWidth;
    min.y = body->position.y - halfHeight;

    max.x = body->position.x + halfWidth;
    max.y = body->position.y + halfHeight;

    int32t aabbID = createAABB(min, max, body);
    body->aabbID = aabbID;
}
