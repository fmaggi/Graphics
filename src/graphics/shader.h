#ifndef SHADER_H
#define SHADER_H

#include "glm/glm.hpp"

typedef struct shader Shader;

Shader* createShader(const char* vertexPath, const char* fragmentPath);
void destroyShader(Shader* shader);

void useShader(Shader* shader);
void shaderSetUniformMat4(Shader* shader, glm::mat4, const char* name);
void shaderSetTextureSlot(Shader *shader, const char* name);

#endif // SHADER_H
