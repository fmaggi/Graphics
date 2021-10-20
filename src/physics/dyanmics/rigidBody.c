#include "rigidBody.h"

#include "physics/collisions/aabb.h"

#include "log/log.h"

Body bodies[32];
static uint32_t current = 0;

struct RestingContact
{
    Body *a, *b;
    vec2s normal;
};

int collide(Body* a, Body* b, vec2s minSeparation, struct RestingContact* c)
{
    vec2s separation = (vec2s){
        {fabs(a->position.x - b->position.x), fabs(a->position.y - b->position.y)}
    };
    // offset = (minSeparation - separation)
    vec2s offset = glms_vec2_add(minSeparation, glms_vec2_negate(separation));

    int aOnTop = (a->position.y - b->position.y) > 0 ? 1 : -1;
    vec2s normal = (vec2s){
        {-(offset.x < offset.y), aOnTop*(offset.x > offset.y)}
    };

    vec2s vA = a->speed;
    vec2s vB = b->speed;

    vec2s dv = glms_vec2_add(vA, glms_vec2_negate(vB));

    float sNormal = glms_vec2_dot(dv, normal);
    if (-sNormal < 0.00001) // at resting contact
    {
        c->a = a;
        c->b = b;
        c->normal = normal;
        return 1;
    }

    vec2s dvNormal = glms_vec2_scale(normal, sNormal);

    vec2s vCorrection = glms_vec2_scale(normal, -sNormal*0.5);
    vec2s dvCorrection = glms_vec2_add(vCorrection, glms_vec2_negate(dvNormal));
    if (a->type == Dynamic)
    {
        a->speed = glms_vec2_add(a->speed, dvCorrection);
    }
    if (b->type == Dynamic)
    {
        b->speed = glms_vec2_add(b->speed, glms_vec2_negate(dvCorrection));
    }
    return 0;       
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

    struct RestingContact contacts[32];
    int cCount = 0;

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
            if (collide(a, b, minSeparation, &contacts[cCount]))
            {
                cCount++;
            }
        }
        calls++;
    }
    LOG_INFO_DEBUG("collide calls: %i\n", calls);

    for (int i = 0; i < current; i++)
    {
        if (bodies[i].type == Static)
            continue;

        vec2s impulse = bodies[i].impulse;
        impulse.y -= 10/ts; // gravity hack
        bodies[i].impulse = impulse;
        vec2s speed = glms_vec2_scale( bodies[i].impulse, ts);
        bodies[i].speed = glms_vec2_add(bodies[i].speed, speed);
    }

    // handle resting contacts. TODO correct this method. Doesnt really work
    for (int i = 0; i < cCount; i++)
    {
        vec2s normal = contacts[i].normal;
        Body *a = contacts[i].a;
        Body *b = contacts[i].b;
        
        if (a->type == Dynamic)
        {
            vec2s iA = a->impulse;
            float iAnormal = glms_vec2_dot(normal, iA);

            vec2s di = glms_vec2_scale(normal, -iAnormal);
            a->impulse = glms_vec2_add(a->impulse, di);
        }

        if (b->type == Dynamic)
        {
            vec2s iB = b->impulse;
            float iBnormal = glms_vec2_dot(normal, iB);

            vec2s di = glms_vec2_scale(normal, -iBnormal);
            b->impulse = glms_vec2_add(b->impulse, di);
        }
    }

    for (int i = 0; i < current; i++)
    {
        if (bodies[i].type == Static)
            continue;

        vec2s dx = glms_vec2_scale(bodies[i].speed, ts);
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
