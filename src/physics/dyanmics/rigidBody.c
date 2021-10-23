#include "rigidBody.h"

#include "physics/collisions/aabb.h"

#include "log/log.h"

Body bodies[32];
static uint32_t current = 0;

struct Contact
{
    Body *a, *b;
    vec2s normal, minSeparation, penetration;
};

void collide(Body* a, Body* b, vec2s minSeparation, struct Contact* c)
{
    int aOnTop = (a->position.y - b->position.y) > 0 ? -1 : 1;
    vec2s separation;
    separation.x = fabs(b->position.x - a->position.x);
    separation.y = aOnTop ? fabs(b->position.y - a->position.y) : fabs(a->position.y - b->position.y);
    // penetration = (separation - minSeparation)
    vec2s penetration = glms_vec2_add(separation, glms_vec2_negate(minSeparation));

    vec2s normal = (vec2s){
        {(penetration.x > penetration.y), aOnTop*(penetration.x < penetration.y)}
    };

    c->a = a;
    c->b = b;
    c->normal = normal;
    c->minSeparation = minSeparation;
    c->penetration = penetration;  
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

    struct Contact contacts[32];
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
            collide(a, b, minSeparation, &contacts[cCount++]);
        }
        calls++;
    }
    LOG_INFO_DEBUG("collide calls: %i\n", calls);

    // This part of the code was inspired by erin catto's Box2d physiscs engine. (https://www.box2d.org/)

    //integrate velocity
    for (int i = 0; i < current; i++)
    {
        if (bodies[i].type == Static)
            continue;
        vec2s impulse = bodies[i].impulse;
        impulse.y -= 10/ts;
        bodies[i].impulse = impulse;
        vec2s speed = glms_vec2_scale(bodies[i].impulse, ts);
        bodies[i].speed = glms_vec2_add(bodies[i].speed, speed);
        bodies[i].impulse = GLMS_VEC2_ZERO;
    }

    // solve velocity constraints
    for (int i = 0; i < cCount; i++)
    {
        Body *a = contacts[i].a;
        Body *b = contacts[i].b;

        vec2s normal = contacts[i].normal;
        vec2s tangent = (vec2s){
            {-normal.y, normal.x}
        };

        vec2s dv = glms_vec2_add(a->speed, glms_vec2_negate(b->speed));

        float lamdat = glms_vec2_dot(dv, tangent) * (0.12); // friction
        vec2s pt = glms_vec2_scale(tangent, lamdat);

        float lamdan = glms_vec2_dot(dv, normal) * 1.5; // restitution
        vec2s pn = glms_vec2_scale(normal, lamdan);

        vec2s p = glms_vec2_add(pt, pn);

        if (a->type == Dynamic)
        {
            a->speed = glms_vec2_add(a->speed, glms_vec2_negate(p));
        }
        if (b->type == Dynamic)
        {
            b->speed = glms_vec2_add(b->speed, p);
        }    
    }

    // integrate position
    for (int i = 0; i < current; i ++)
    {
        if (bodies[i].type == Static)
            continue;

        vec2s dx = glms_vec2_scale(bodies[i].speed, ts);
        if (glms_vec2_dot(dx, dx) > 100)
        {
            float r = 10 / glms_vec2_norm(dx);
            bodies[i].speed = glms_vec2_scale(bodies[i].speed, r);
            dx = glms_vec2_scale(bodies[i].speed, ts);
        }
        bodies[i].position = glms_vec3_add(bodies[i].position, (vec3s){{dx.x, dx.y, 0}});
    }


    // solve contact at rest position constraints
    for (int i = 0; i < cCount; i++)
    {
        Body *a = contacts[i].a;
        Body *b = contacts[i].b;

        vec2s normal = contacts[i].normal;

        vec2s penetration = contacts[i].penetration;

        vec2s offset = glms_vec2_mul(penetration, normal);

        if (a->type == Dynamic)
        {
            a->position = glms_vec3_add(a->position, ((vec3s){{offset.x, offset.y, 0}}));
        }
        if (b->type == Dynamic)
        {
            b->position = glms_vec3_add(b->position, glms_vec3_negate((vec3s){{offset.x, offset.y, 0}}));
        }
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
