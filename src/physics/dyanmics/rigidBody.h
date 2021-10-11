#ifndef BODY_H
#define BODY_H

#include "cglm/struct.h"
#include "util/types.h"

enum BodyType
{
    Static = 0, Dynamic
};

typedef struct
{
    vec2s min, max;
} AABB;

typedef struct
{
    vec3s position;
    vec2s speed, forces;
    int32t flags;

    enum BodyType type;

    int32t aabbID;
} Body;

int32t createBody(vec3s position, enum BodyType type, AABB aabb, int32t flags);

#endif