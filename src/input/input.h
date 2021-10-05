#ifndef INPUT_H
#define INPUT_H

#include "entity/entity.h"

void initInput();
void handleInput(Entity* movable, double timestep);

#endif