#ifndef SHADER_H
#define SHADER_H

#include "glm/glm.hpp"
#include <string>

#include <unordered_map>

#include "shaderData.h"

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

struct Shader
{
    Shader(const char* vertexPath, const char* fragmentPath, ShaderProps props = defaultShaderProps);
    ~Shader();

    void Bind();
    void Unbind();

    void SetUniformMat4(glm::mat4, const char* name);
    void SetIntArray(const char* name, size_t size);

    int32_t GetUniformLocation(const char* name);

    UniformInfo GetUniformInfo(const std::string& name);

    // return how many uniforms in shader
    uint32_t Tell();

    ShaderProps m_shaderProps;
private:
    uint32_t m_ProgramID;
    uint32_t m_VertexID;
    uint32_t m_FragmentID;

    std::unordered_map<std::string, UniformInfo> m_uniforms{};
};

#endif // SHADER_H
