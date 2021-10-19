#ifndef RENDERER_H
#define RENDERER_H

#include "cglm/struct.h"

#define MAX_SHADER 2
enum ShaderType
{
    basicShader = 0,
    uvShader
};

void initRenderer();
void destroyRenderer();

void rendererSubmit(mat4s transform, vec3s color, float texIndex);

void startFrame();
void endFrame();

void rendererChangeMode();
void rendererSetShader(enum ShaderType type);

#endif