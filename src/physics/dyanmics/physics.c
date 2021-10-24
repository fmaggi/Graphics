#include "physics.h"

#include "physics/collisions/aabb.h"
#include "physics/collisions/contact.h"

#include "log/log.h"

struct Simulation
{
    Body bodies[32];
    uint32_t currentBody;
    int32_t gravity;
};

static struct Simulation simulation;

void initPhysics(int32_t gravity)
{
    simulation.currentBody = 0;
    simulation.gravity = gravity;
}

void stepPhysics(double ts)
{
    static struct ContactStack stack;
    struct Contact contacts[32] = {0};
    stack.contacts = contacts;
    stack.count = 0;
    stack.size = 32;

    for (int i = 0; i < simulation.currentBody; i++)
    {
        Body* b = simulation.bodies + i;
        if (b->type == Dynamic)
            updateAABB(b->aabbID, b->position);
    }

    sweepAndPrune(&stack);

    int calls = 0;
    for (struct Contact* c = stack.contacts; c->next != NULL; c = c->next)
    {
        Body* a = c->left;
        Body* b = c->right;
        if (b->type == Static && a->type == Static)
        {
            struct Contact* d = c;
            destroyContact(&stack, d);
            continue;
        }

        calls++;
        bool collided = testOverlap(a->aabbID, b->aabbID);
        if (!collided)
        {
            struct Contact* d = c;
            destroyContact(&stack, d);
            continue;
        }

        collide(c);
    }
    LOG_INFO_DEBUG("collide calls: %i\n", calls);

    // This part of the code was inspired by erin catto's Box2d physiscs engine. (https://www.box2d.org/)

    //integrate velocity
    for (int i = 0; i < simulation.currentBody; i++)
    {
        Body* b = simulation.bodies + i;
        if (b->type == Static)
            continue;

        b->impulse.y += simulation.gravity;

        vec2s speed = glms_vec2_scale(b->impulse, ts);
        b->speed = glms_vec2_add(b->speed, speed);

        b->impulse = GLMS_VEC2_ZERO;
    }

    // solve velocity constraints
    for (struct Contact* c = stack.contacts; c->next != NULL; c = c->next)
    {
        Body *a = c->left;
        Body *b = c->right;

        vec2s normal = c->normal;
        vec2s tangent = (vec2s){
            {-normal.y, normal.x}
        };

        vec2s dv = glms_vec2_add(b->speed, glms_vec2_negate(a->speed));

        float lamdat = glms_vec2_dot(dv, tangent) * (0.12); // friction
        vec2s pt = glms_vec2_scale(tangent, lamdat);

        float lamdan = glms_vec2_dot(dv, normal) * 1.5; // restitution
        vec2s pn = glms_vec2_scale(normal, lamdan);

        vec2s p = glms_vec2_add(pt, pn);

        if (a->type == Dynamic)
        {
            a->speed = glms_vec2_add(a->speed, p);
            if (fabs(a->speed.x) < 30)
                a->speed.x = 0;
            if (fabs(a->speed.y) < 30)
                a->speed.y = 0;

        }
        if (b->type == Dynamic)
        {
            b->speed = glms_vec2_add(b->speed, glms_vec2_negate(p));
            if (fabs(b->speed.x) < 30)
                b->speed.x = 0;
            if (fabs(b->speed.y) < 30)
                b->speed.y = 0;
        }
    }

    // integrate position
    for (int i = 0; i < simulation.currentBody; i ++)
    {
        Body* b = simulation.bodies + i;
        if (b->type == Static)
            continue;

        vec2s dx = glms_vec2_scale(b->speed, ts);
        b->position = glms_vec3_add(b->position, (vec3s){{dx.x, dx.y, 0}});
    }

    // solve position constraints
    for (struct Contact* c = stack.contacts; c->next != NULL; c = c->next)
    {
        Body *a = c->left;
        Body *b = c->right;

        vec2s normal = c->normal;
        vec2s penetration = c->penetration;

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

Body* createBody(vec3s position, enum BodyType type, CollisionCallback callback, void* userData, uint32_t userFlags)
{
    Body* body= simulation.bodies + simulation.currentBody++;

    body->position = position;
    body->type = type;
    body->onCollision = callback;
    body->userData = userData;
    body->userFlags = userFlags;

    body->impulse = GLMS_VEC2_ZERO;
    body->speed = GLMS_VEC2_ZERO;
    body->aabbID = -1;

    return body;
}

