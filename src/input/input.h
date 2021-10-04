#ifndef INPUT_H
#define INPUT_H

#include "entity/entity.h"
#include "graphics/gfx.h"

void initInput(GLFWwindow* window);
void handleInput(Entity* movable, double timestep);

#endif