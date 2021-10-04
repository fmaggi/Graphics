#ifndef RENDERER_H
#define RENDERER_H

#include "world/world.h"

#include "cglm/struct.h"

#define MAX_SHADER 1
enum ShaderType
{
    basicShader
};

typedef struct _renderer  Renderer;

Renderer* createRenderer(mat4s proj);
void destroyRenderer(Renderer* r);

void render(Renderer* r, World* w);

void rendererChangeMode();

void rendererSetProjectionMatrix(Renderer* r, mat4s proj);

void rendererUseShader(Renderer* r, enum ShaderType type);

void prepareRenderer();

#endif