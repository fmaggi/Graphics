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
    vec3s position;
    vec2s speed, forces;
    int32t flags;

    enum BodyType type;

    int32t aabbID;
} Body;

void createBody(vec3s position, enum BodyType type, int32t flags);
void addAABB(Body* body, float halfWidth, float halfHeight);

#endif