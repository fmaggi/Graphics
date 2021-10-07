#ifndef SHADER_H
#define SHADER_H

#include "cglm/struct.h"

typedef struct _shader 
{
    unsigned int vertexID;
    unsigned int fragmentID;
    unsigned int programID;
} Shader;

Shader createShader(const char* vertexPath, const char* fragmentPath);
void destroyShader(Shader shader);

void useShader(Shader shader);
void shaderSetUniformMat4(Shader shader, mat4s mat, const char* name);
void shaderSetTextureSlot(Shader shader, unsigned int slot, const char* name);

#endif // SHADER_H