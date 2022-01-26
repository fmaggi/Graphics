#ifndef SOLVER_H
#define SOLVER_H

#include "rigidBody.h"
#include "physics/collisions/contact.h"

#include "glm/glm.hpp"

void Solve(Body* bodies, Contact* contacts, glm::vec2 gravity, float ts);

#endif
