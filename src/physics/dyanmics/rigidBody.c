#include "rigidBody.h"

#include "physics/collisions/aabb.h"

#include "log/log.h"

Body bodies[32];
static uint32_t current = 0;

void collide(Body* a, Body* b, vec2s minSeparation, double ts)
{

    vec2s separation = (vec2s){
        {fabs(a->position.x - b->position.x), fabs(a->position.y - b->position.y)}
    };
    // offset = (minSeparation - separation)
    vec2s offset = glms_vec2_add(minSeparation, glms_vec2_negate(separation));

    vec2s normal = (vec2s){
        {offset.x < offset.y, offset.x > offset.y}
    };

    offset = glms_vec2_mul(offset, normal);
    int aOnTop = (a->position.y - b->position.y) > 0 ? -1 : 1;
    offset.y *= aOnTop;

    offset = glms_vec2_scale(offset, 1/(ts*ts)); // im pretty sure this is a hack

    if (a->type == Dynamic)
    {
        a->impulse = glms_vec2_add(a->impulse, glms_vec2_negate(offset));
    }
    if (b->type == Dynamic)
    {
        b->impulse = glms_vec2_add(b->impulse, offset);
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
            collide(a, b, minSeparation, ts);
        }
        calls++;
    }
    LOG_INFO_DEBUG("collide calls: %i\n", calls);

    for (int i = 0; i < current; i++)
    {
        if (bodies[i].type == Static)
            continue;

        vec2s impulse = bodies[i].impulse;
        impulse.y -= 100; // gravity hack
        vec2s speed = glms_vec2_scale(impulse, ts);
        speed = glms_vec2_add(bodies[i].speed, speed);
        bodies[i].speed = speed;
        vec2s dx = glms_vec2_scale(speed, ts);
        bodies[i].position = glms_vec3_add(bodies[i].position, (vec3s){{dx.x, dx.y, 0}});
        bodies[i].impulse = GLMS_VEC2_ZERO;
    }
}

Body* createBody(vec3s position, enum BodyType type, int flags)
{
    Body* body= &bodies[current++];

    body->position = position;
    body->type = type;

    body->flags = flags;
    body->impulse = GLMS_VEC2_ZERO;
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
