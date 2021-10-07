#ifndef RENDERER_H
#define RENDERER_H

#include "world/world.h"
#include "camera.h"

#define MAX_SHADER 2
enum ShaderType
{
    basicShader = 0,
    uvShader
};

void createRenderer();
void destroyRenderer();

void render(World* w);

void startFrame(Camera* c);
void endFrame();

//void drawIndexed();

void rendererChangeMode();
void rendererSetShader(enum ShaderType type);

#endif