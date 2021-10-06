#ifndef RENDERER_H
#define RENDERER_H

#include "world/world.h"
#include "camera.h"

void createRenderer();
void destroyRenderer();

void render(World* w);

void startFrame(Camera* c);
void endFrame();

//void drawIndexed();

void rendererChangeMode();
void prepareRenderer();

#endif