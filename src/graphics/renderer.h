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

void pushQuad(vec3s position, float rotation, vec2s scale, vec3s color, float texIndex);

void startFrame();
void endFrame();

void rendererChangeMode();
void rendererSetShader(enum ShaderType type);

void setViewport(int width, int height);

#endif
