#include "rigidBody.h"

struct
{
    Body* bodies;
    int32t count;
} w = {NULL, 0};

int32t createBody(vec3s position, enum BodyType type, AABB aabb, int32t flags)
{
    if (w.bodies == NULL)
    {
        w.bodies = malloc(sizeof(Body) * 16);
    }
    Body body = w.bodies[w.count];
    body.position = position;
    body.forces = (vec2s){0, 0};
    body.speed = (vec2s){0, 0};
    body.flags = flags;

    return w.count++;
}

