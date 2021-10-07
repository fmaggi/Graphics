#include "shader.h"

#include "gfx.h"

#include "log/log.h"
#include "stdlib.h"
#include "string.h"

unsigned int compileShader(const char* path, unsigned int type)
{
    char buf[256] = {0};
    strcpy(buf, "res/shaders/");
    strcat(buf, path);
    FILE* srcFile = fopen(buf, "r");
    if (!srcFile)
    {
        LOG_WARN("Failed to open %s\n", buf);
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

    unsigned int shader = glCreateShader(type);
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
        LOG_ERROR("%s shader compilation:\n", s);
        LOG("  %s\n", infoLog);
        exit(-1);
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
        LOG_ERROR("Shader linking:\n");
        LOG("  %s\n", infoLog);
        exit(-1);
    }
    return shaderProgram;
}

Shader createShader(const char* vertexPath, const char* fragmentPath)
{
    Shader shader;
    shader.vertexID = compileShader(vertexPath, GL_VERTEX_SHADER);
    shader.fragmentID = compileShader(fragmentPath, GL_FRAGMENT_SHADER);
    shader.programID = linkShader(shader.vertexID, shader.fragmentID);
    return shader;
}

void destroyShader(Shader shader)
{
    glDeleteShader(shader.vertexID);
    glDeleteShader(shader.fragmentID);
    glDeleteProgram(shader.fragmentID);
}

int getUniformLocation(Shader shader, const char* name)
{
    int location = glGetUniformLocation(shader.programID, name);
    if (location == -1)
        LOG_WARN("Invalid uniform: %s\n", name);
    return location;
}

void shaderSetUniformMat4(Shader shader, mat4s mat, const char* name)
{
    glUseProgram(shader.programID);
    int location = getUniformLocation(shader, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, (float *)&mat.raw);
}


void shaderSetTextureSlot(Shader shader, unsigned int slot, const char* name)
{
    glUseProgram(shader.programID);
    int location = getUniformLocation(shader, name);
    glUniform1i(location, slot);
}

void useShader(Shader shader)
{
    glUseProgram(shader.programID);
}

