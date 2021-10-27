#ifndef RENDERER_H
#define RENDERER_H

#include "cglm/struct.h"
#include "entity/entity.h"

#define MAX_SHADER 2
enum ShaderType
{
    basicShader = 0,
    uvShader
};

void initRenderer();
void destroyRenderer();

void rendererSubmit(EntityID id);

void startFrame();
void endFrame();

void rendererChangeMode();
void rendererSetShader(enum ShaderType type);

#endif
