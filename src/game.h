#ifndef GAME_H
#define GAME_H

#include "graphics/window.h"

typedef struct _game Game;

int setUpGame();
void runGame();
void destroyGame();

Window* getWindow();

#endif