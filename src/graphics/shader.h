#ifndef SHADER_H
#define SHADER_H

#include "glm/glm.hpp"
#include <string>

#include "shaderUniforms.h"

#include <iostream>
#include "log/log.h"

enum class BlendFunc
{
    None = 0,
    Aplha_Src_OneMinusSrc
};

enum class FillMode
{
    Fill = 0,
    Line
};

struct ShaderProps
{
    BlendFunc blend = BlendFunc::None;
    bool depthTest = false;
    FillMode fill = FillMode::Fill;
};

extern ShaderProps defaultShaderProps;

// This shader class probably does a little to much, but whatevs
// Shaders (obviously)
// general pipeline stuff like wether depth test is enabled
// material stuff
struct Shader
{
    Shader(const char* vertexPath, const char* fragmentPath, ShaderProps props = defaultShaderProps);
    ~Shader();

    void Bind();
    void Unbind();

    void CommitUniforms();

    int32_t GetUniformLocation(const char* name);

    UniformInfo GetUniformInfo(const std::string& name);
    template<typename T>
    void SetData(const std::string& uniformName, const T& value);

    ShaderProps shaderProps;

    uint32_t ProgramID;
    uint32_t VertexID;
    uint32_t FragmentID;
private:
    void introspection();
    Uniforms uniforms;
};

template<typename T>
void Shader::SetData(const std::string& uniformName, const T& value)
{
    UniformInfo info = GetUniformInfo(uniformName);
    if (info.location == -1)
    {
        LOG_INFO("Invalid uniform name!");
        return;
    }

    if (info.type != shaderDataType<T>())
    {
        LOG_INFO("Invalid data type!");
        return;
    }

    uniforms.data.Write(value, info.bytesOffset);
}

#endif // SHADER_H
