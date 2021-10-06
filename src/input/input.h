#ifndef INPUT_H
#define INPUT_H

#include "entity/entity.h"
#include "graphics/camera.h"

void initInput();
void handleInput(Camera* movable, double timestep);

#endif