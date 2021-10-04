#ifndef RENDERER_H
#define RENDERER_H

#include "vao.h"
#include "buffer.h"

#include "world/world.h"

#define MAX_SHADER 1
enum ShaderType
{
    basicShader
};

typedef struct _renderer  Renderer;

Renderer* createRenderer();
void destroyRenderer(Renderer* r);

void render(World* w);

void rendererChangeMode();

void rendererUseShader(Renderer* r, enum ShaderType type);

void prepareRenderer();

#endif