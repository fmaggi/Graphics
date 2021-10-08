#ifndef RENDERER_H
#define RENDERER_H

#define MAX_SHADER 2
enum ShaderType
{
    basicShader = 0,
    uvShader
};

void createRenderer();
void destroyRenderer();

void render();

void startFrame();
void endFrame();

//void drawIndexed();

void rendererChangeMode();
void rendererSetShader(enum ShaderType type);

#endif