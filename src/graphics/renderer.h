#ifndef RENDERER_H
#define RENDERER_H

#include "world/world.h"
#include "camera.h"

#define MAX_SHADER 1
enum ShaderType
{
    basicShader
};

void createRenderer();
void destroyRenderer();

void render(World* w);

void startFrame(Camera* c);
void endFrame();

//void drawIndexed();

void rendererChangeMode();

#endif