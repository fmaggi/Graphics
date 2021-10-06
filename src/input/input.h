#ifndef INPUT_H
#define INPUT_H

#include "entity/entity.h"
#include "graphics/camera.h"

#include "keys.h"

void initInput();
void changeInput(int keyUp, int keyDown, int keyLeft, int keyRight);

void handleInput(Camera* movable, double timestep);

#endif