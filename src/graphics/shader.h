#ifndef SHADER_H
#define SHADER_H

#include "cglm/struct.h"

typedef struct _shader Shader;

Shader* createShader(const char* vertexPath, const char* fragmentPath);
void destroyShader(Shader* shader);

void useShader(Shader* shader);
void shaderSetUniformMat4(Shader* shader, mat4s mat, const char* name);

#endif // SHADER_H