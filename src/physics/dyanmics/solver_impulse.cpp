#include "solver.h"

#include "physics/collisions/aabb.h"

#define VEL_ITERATIONS 5
#define POS_ITERATIONS 2

static float max(float a, float b)
{
    return a > b ? a : b;
}

// static float min(float a, float b)
// {
//     return a < b ? a : b;
// }

static void integrate_forces(Body* bodies, glm::vec2 gravity, float ts)
{
    Body* b = bodies;
    while (b != nullptr)
    {
        if (!b->mass || b->type == BodyType::Static)
        {
            b = b->next;
            continue;
        }

        glm::vec2 v = b->velocity;
        v += ts * (gravity + b->force * b->Imass);

        b->velocity = v;
        b->force = {0, 0};

        b = b->next;
    }
}

static void integrate_velocities(Body* bodies, float ts)
{
    Body* b = bodies;
    while (b != nullptr)
    {
        if (b->type == BodyType::Static)
        {
            b = b->next;
            continue;
        }

        glm::vec2 dx = ts * b->velocity;
        b->translation += dx;

        UpdateAABB(b->aabb, b->translation);

        b = b->next;
    }
}

static void solve_velocity_constraints(Contact* contact)
{
    Contact* c = contact;
    while (c)
    {
        Body *a = c->left;
        Body *b = c->right;

        glm::vec2 normal = c->normal;
        glm::vec2 tangent = {
            -normal.y, normal.x
        };
        float normalMass = c->normalMass;

        glm::vec2 dv = b->velocity - a->velocity;

        float tangentImpulse = -glm::dot(dv, tangent) * 0.0; // friction
        glm::vec2 pt = tangent * tangentImpulse;

        float dvn = glm::dot(dv, normal);
        float normalImpulse = dvn * -normalMass * 2; // restitution

        float newImpulse = max(normalImpulse + c->normalImpulse, 0.0f);
        normalImpulse = newImpulse - c->normalImpulse;
        c->normalImpulse = newImpulse;

        glm::vec2 pn = normal * normalImpulse;

        glm::vec2 p = pt + pn;

        a->velocity -= p * a->Imass;
        b->velocity += p * b->Imass;
        c = c->next;
    }
}

static void solve_position_constraints(Contact* contacts)
{
    Contact* c = contacts;
    while(c)
    {
        Body *a = c->left;
        Body *b = c->right;

        glm::vec2 normal = c->normal;
        float K = 1 / c->normalMass;

        glm::vec2 penetration = getPenetration(a, b, c->minSeparation);

        float penetrationNormal = fabs(glm::dot(penetration, normal));

        // black magic from erin catto at box2d. Dont know why we divide penetration with normal mass
        // this fixes "vibrations" when one box stands on another
        // I guess is to make them separate from each other relative to their masses
        // also, why is it an impulse?
        float impulse = K > 0.0f ? -penetrationNormal / K : 0.0f;

        glm::vec2 res = impulse * normal;

        a->translation += res * a->Imass;
        b->translation -= res * b->Imass;

        UpdateAABB(a->aabb, a->translation);
        UpdateAABB(b->aabb, b->translation);

        c = c->next;
    }
}

void Solve(Body* bodies, Contact* contacts, glm::vec2 gravity, float ts)
{
    integrate_forces(bodies, gravity, ts);

    for (int _ = 0 ; _ < VEL_ITERATIONS; ++_)
        solve_velocity_constraints(contacts);

    integrate_velocities(bodies, ts);

    for (int _ = 0 ; _ < POS_ITERATIONS; ++_)
        solve_position_constraints(contacts);
}
