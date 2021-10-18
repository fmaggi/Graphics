#ifndef BODY_H
#define BODY_H

#include "cglm/struct.h"

enum BodyType
{
    Static = 0, Dynamic
};

typedef struct
{
    vec3s position;
    vec2s speed, forces;
    int flags;

    enum BodyType type;

    int aabbID;
} Body;

Body* createBody(vec3s position, enum BodyType type, int flags);
void addAABB(Body* body, float halfWidth, float halfHeight);

//temporary
void update(double ts);

#endif