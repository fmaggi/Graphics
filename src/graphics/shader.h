#ifndef SHADER_H
#define SHADER_H

typedef struct _shader Shader;

Shader* createShader(const char* vertexPath, const char* fragmentPath);
void destroyShader(Shader* shader);

void useShader(Shader* shader);

#endif // SHADER_H