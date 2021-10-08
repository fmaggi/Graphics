#ifndef INPUT_H
#define INPUT_H

#include "entity/entity.h"

#include "keys.h"

void initInput();
void changeInput(int keyUp, int keyDown, int keyLeft, int keyRight);

void handleInput(EntityID movable, double timestep);

#endif