#include "rigidBody.h"

#include "physics/collisions/aabb.h"

#include "log/log.h"


Body bodies[32];
static int32t current = 0;

void update(double ts)
{
    struct CollisionStack c;
    c.collisions = malloc(sizeof(void*) * current * current); // worst case
    c.count = 0;

    sweepAndPrune(&c);

    for (int i = 0; i < c.count; i++)
    {
        Body* a = c.collisions[i].a;
        Body* b = c.collisions[i].b;
        int didCollide = collide(a->aabbID, b->aabbID);

        if (didCollide)
            LOG_INFO("Collision\n");
        else
            LOG_INFO("Not\n");
    }
}

Body* createBody(vec3s position, enum BodyType type, int32t flags)
{
    Body* body= &bodies[current++];

    body->position = position;
    body->type = type;

    body->flags = flags;
    body->forces = GLMS_VEC2_ZERO;
    body->speed = GLMS_VEC2_ZERO;

    return body;
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

    int32t aabbID = createAABB2(min, max, body);
    body->aabbID = aabbID;
}
