#ifndef AABB_H
#define AABB_H

#include "glm/glm.hpp"

int32_t createAABB2(glm::vec2 center, glm::vec2 halfExtents, void* bodyID);
void updateAABB(int32_t id, glm::vec3 position);

struct ContactStack; // forward declaration
void sweepAndPrune(struct ContactStack* results);

bool testOverlap(int32_t aID, int32_t bID);

#endif
