#ifndef COLLISION_H
#define COLLISION_H

#include "entity/entity.h"
#include "cglm/struct.h"

void screenVerticalDivisionsCollision(TransformComponent* transforms, PhysicsComponent* physics, int count, int screenWidth, int screenHeight);

#endif