#ifndef PHYSICS_H
#define PHYSICS_H

#include "rigidBody.h"

void initPhysics(float gravity);
void stepPhysics(double ts);

Body* createBody(glm::vec3 position, enum BodyType type, CollisionCallback callback, void* userData, uint32_t userFlags);

#endif
