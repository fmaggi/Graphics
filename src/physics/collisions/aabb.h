#ifndef AABB_H
#define AABB_H

#include "cglm/struct.h"

int32_t createAABB2(vec2s center, vec2s halfExtents, void* bodyID);
void updateAABB(int32_t id, vec3s position);

struct ContactStack; // forward declaration
void sweepAndPrune(struct ContactStack* results);

int32_t testOverlap(int32_t aID, int32_t bID);

#endif
