#ifndef BODY_H
#define BODY_H

#include "cglm/struct.h"

enum BodyType
{
    Static = 0, Dynamic
};

typedef struct body
{
    vec3s position;
    vec2s speed, impulse;

    enum BodyType type;
    int aabbID;

    void (*onCollision)(struct body* self, struct body* other);
    void* userData;
    uint32_t userFlags;
} Body;

typedef void (*CollisionCallback)(Body* self, Body* other);

Body* createBody(vec3s position, enum BodyType type, CollisionCallback callback, void* userData, uint32_t userFlags);
void addAABB(Body* body, float halfWidth, float halfHeight);

//temporary
void update(double ts);

#endif
