#ifndef RENDERER_H
#define RENDERER_H

#include "vao.h"
#include "buffer.h"

#include "world/world.h"

#include "cglm/struct.h"

#define MAX_SHADER 1
enum ShaderType
{
    basicShader
};

typedef struct _renderer  Renderer;

Renderer* createRenderer();
void destroyRenderer(Renderer* r);

void render(Renderer* r, World* w);

void rendererChangeMode();

unsigned int rendererUseShader(Renderer* r, enum ShaderType type);
void rendererSetProjectionMatrix(Renderer* r, mat4s proj);

void prepareRenderer();

#endif