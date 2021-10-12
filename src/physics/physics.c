#include "physics.h"

#include "collision.h"

#include "cglm/struct.h"

#include "entity/entity.h"

#include "log/log.h"

static int screenWidth;
static int screenHeight;

void initPhysics(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
}

void setPhysicsWidthHeight(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
}

void updateForces(TransformComponent* t, PhysicsComponent* p, double ts)
{
    vec2s speed = glms_vec2_scale(p->force, ts);
    float gravity = -1 * ts;
    speed.y += gravity;
    p->speed = glms_vec2_add(speed, p->speed);
    vec2s movement = glms_vec2_scale(p->speed, ts);

    t->position = glms_vec3_add(t->position, (vec3s){movement.x, movement.y, 0});
    p->force = (vec2s){0, 0};
}

void updatePhysics(double ts)
{
    unsigned int count = getEntityCount();

    PhysicsComponent* physics = registerView(Physics);
    TransformComponent* transforms = registerView(Transform); 

    screenVerticalDivisionsCollision(transforms, physics, count, screenWidth, screenHeight);

    for (int i = 0; i < count; i++)
    {
        if (physics[i].flags & ACTIVE && hasComponent(i, Transform) && physics[i].flags & DYNAMIC)
        {
            updateForces(&transforms[i], &physics[i], ts);
        }
    }
}