#ifndef PHYSICS_H
#define PHYSICS_H

#include "rigidBody.h"

void initPhysics();
void stepPhysics(double ts);

Body* createBody(vec3s position, enum BodyType type, CollisionCallback callback, void* userData, uint32_t userFlags);

#endif
