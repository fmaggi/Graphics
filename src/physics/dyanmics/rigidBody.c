#include "rigidBody.h"

#include "physics/collisions/aabb.h"

#include "log/log.h"

Body bodies[32];
static uint32_t current = 0;

void collide(Body* a, Body* b, vec2s minSeparation)
{
    vec2s separation = (vec2s){
        {fabs(a->position.x - b->position.x), fabs(a->position.y - b->position.y)}
    };
    //normal to contact surface
    vec2s normal = (vec2s){
        {separation.x > separation.y, separation.x < separation.y}
    };
    // offset = normal * (minSeparation - separation)
    vec2s offset = glms_vec2_mul(normal,
                   glms_vec2_add(minSeparation, 
                   glms_vec2_negate(separation)));

    int aOnTop = a->position.y - b->position.y > 0;
    if (a->type == Dynamic) // a is always left but dont know if top or bottom
    {
        a->position.x -= offset.x;
        a->position.y += offset.y * (aOnTop) - offset.y * (!aOnTop);
    }
    if (b->type == Dynamic)
    {
        b->position.x += offset.x;
        b->position.y -= offset.y * (aOnTop) - offset.y * (!aOnTop);
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
        {
            collide(a, b, minSeparation);
            calls++;
        }
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

    vec2s center = (vec2s){{body->position.x, body->position.y}};
    vec2s halfExtents = (vec2s){{halfWidth, halfHeight}};

    body->aabbID = createAABB2(center, halfExtents, body);
}
