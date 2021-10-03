#include "shader.h"

#include "gfx.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct _shader 
{
    unsigned int vertexID;
    unsigned int fragmentID;
    unsigned int programID;
} Shader;

unsigned int compileShader(const char* path, unsigned int type)
{
    char buf[256];
    strcpy(buf, "res/");
    strcat(buf, path);
    FILE* srcFile = fopen(buf, "r");
    if (!srcFile)
    {
        printf("Failed to open %s\n", path);
    }

    fseek(srcFile, 0L, SEEK_END);
    size_t size = ftell(srcFile);
    rewind(srcFile);
    char* src = malloc(size * sizeof(char) + 1);
    fread(src, 1, size, srcFile);
    fclose(srcFile);
    src[size] = 0;
    
    const char** srcAddress = (const char**) &src;

    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, srcAddress, NULL);
    glCompileShader(shader);

    free(src);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("COMPILING SHADER ERROR:\n%s\n",infoLog);
    }
    return shader;
}

unsigned int linkShader(unsigned int vertexID, unsigned int fragmentID)
{
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexID);
    glAttachShader(shaderProgram, fragmentID);
    glLinkProgram(shaderProgram);
    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("LINKING SHADER ERROR:\n%s\n",infoLog);
    }
    return shaderProgram;
}

Shader* createShader(const char* vertexPath, const char* fragmentPath)
{
    Shader* shader = malloc(sizeof(Shader));
    shader->vertexID = compileShader(vertexPath, GL_VERTEX_SHADER);
    shader->fragmentID = compileShader(fragmentPath, GL_FRAGMENT_SHADER);
    shader->programID = linkShader(shader->vertexID, shader->fragmentID);
    return shader;
}

void destroyShader(Shader* shader)
{
    glDeleteShader(shader->vertexID);
    glDeleteShader(shader->fragmentID);
    glDeleteProgram(shader->fragmentID);
    free(shader);
}

void useShader(Shader* shader)
{
    glUseProgram(shader->programID);
}

