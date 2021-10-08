#ifndef INPUT_H
#define INPUT_H

#include "entity/entity.h"

#include "keys.h"

typedef int Keycode;
typedef int ButtonCode;

void initInput();
void changeInput(int keyUp, int keyDown, int keyLeft, int keyRight);

void handleInput(EntityID movable, double timestep);

int isKeyPressed(Keycode key);
int isMouseButtonPressed(ButtonCode button);

#endif