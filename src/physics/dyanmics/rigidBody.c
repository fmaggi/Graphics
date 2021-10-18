#include "rigidBody.h"

#include "physics/collisions/aabb.h"

#include "log/log.h"

Body bodies[32];
static uint32_t current = 0;

void collide(Body* a, Body* b, vec2s minSeparation)
{
    vec2s dir;
    dir.x = a->position.x - b->position.x;
    dir.y = a->position.y - b->position.y;
    
    vec2s normal;
    normal.x = fabs(dir.x) > fabs(dir.y);
    normal.y = fabs(dir.x) < fabs(dir.y);
    vec2s offset;
    offset.x = normal.x * (minSeparation.x - fabs(dir.x));
    offset.y = normal.y * (minSeparation.y - fabs(dir.y));
    if (a->type == Dynamic) // a is always left but dont know if top or bottom
    {
        a->position.x -= offset.x;
        a->position.y += offset.y * (dir.y>0) - offset.y * (dir.y<0);
    }
    if (b->type == Dynamic)
    {
        b->position.x += offset.x;
        b->position.y -= offset.y * (dir.y>0) - offset.y * (dir.y<0);
    }
}

void update(double ts)
{
    struct CollisionStack c;
    struct Collision collisions[32];
    c.collisions = collisions;
    c.count = 0;

    for (int i = 0; i < current; i++)
    {
        updateAABB(bodies[i].aabbID, bodies[i].position);
    }

    sweepAndPrune(&c);
    int calls = 0;
    for (int i = 0; i < c.count; i++)
    {
        Body* a = c.collisions[i].left;
        Body* b = c.collisions[i].right;
        vec2s minSeparation = c.collisions[i].minSepareation;
        if (b->type == Static && a->type == Static)
            continue;
        if (testOverlap(a->aabbID, b->aabbID))
            collide(a, b, minSeparation);
        calls++;
    }
    LOG_INFO_DEBUG("collide calls: %i\n", calls);
}

Body* createBody(vec3s position, enum BodyType type, int flags)
{
    Body* body= &bodies[current++];

    body->position = position;
    body->type = type;

    body->flags = flags;
    body->forces = GLMS_VEC2_ZERO;
    body->speed = GLMS_VEC2_ZERO;
    body->aabbID = -1;

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

    int aabbID = createAABB2(min, max, body);
    body->aabbID = aabbID;
}
