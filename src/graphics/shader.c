#include "shader.h"

#include "gfx.h"

#include "log/log.h"
#include "stdlib.h"
#include "string.h"

typedef struct shader
{
    uint32_t vertexID;
    uint32_t fragmentID;
    uint32_t programID;
} Shader;

uint32_t compileShader(const char* path, uint32_t type)
{
    char buf[256] = {0};
    strcpy(buf, "res/shaders/");
    strcat(buf, path);
    FILE* srcFile = fopen(buf, "r");
    if (!srcFile)
    {
        LOG_WARN("Failed to open %s", buf);
        return 0;
    }
    memset(buf, 0, 256);

    fseek(srcFile, 0L, SEEK_END);
    size_t size = ftell(srcFile);
    rewind(srcFile);
    char* src = malloc(size * sizeof(char) + 1);
    fread(src, 1, size, srcFile);
    fclose(srcFile);
    src[size] = 0;

    const char** srcAddress = (const char**) &src;

    uint32_t shader = glCreateShader(type);
    glShaderSource(shader, 1, srcAddress, NULL);
    glCompileShader(shader);

    free(src);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char* s = type == GL_VERTEX_SHADER ? "Vertex" : "Fragment";
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        LOG_ERROR("%s shader compilation:", s);
        LOG("  %s", infoLog);
        exit(-1);
    }
    return shader;
}

uint32_t linkShader(uint32_t vertexID, uint32_t fragmentID)
{
    uint32_t shaderProgram = glCreateProgram();
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
        LOG_ERROR("Shader linking:");
        LOG("  %s", infoLog);
        exit(-1);
    }
    return shaderProgram;
}

Shader* createShader(const char* vertexPath, const char* fragmentPath)
{
    Shader* shader = malloc(sizeof(Shader));
    if(shader == NULL)
        LOG_WARN("Memory allocation failed: shader");
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

int getUniformLocation(Shader* shader, const char* name)
{
    int location = glGetUniformLocation(shader->programID, name);
    if (location == -1)
        LOG_WARN("Invalid uniform: %s", name);
    return location;
}

void shaderSetUniformMat4(Shader* shader, mat4s mat, const char* name)
{
    glUseProgram(shader->programID);
    int location = getUniformLocation(shader, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, (float *)&mat.raw);
}


void shaderSetTextureSlot(Shader* shader,const char* name)
{
    glUseProgram(shader->programID);
    int location = getUniformLocation(shader, name);
    GLint slots[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    glUniform1iv(location, 16, slots);
}

void useShader(Shader* shader)
{
    glUseProgram(shader->programID);
}

