#ifndef RENDERER_H
#define RENDERER_H

#include "world/world.h"
#include "camera.h"

void createRenderer();
void destroyRenderer();

void render(World* w);

void startScene(Camera* c);
void endScene();

//void drawIndexed();

void rendererChangeMode();
void prepareRenderer();

#endif