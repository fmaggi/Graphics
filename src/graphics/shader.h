#ifndef SHADER_H
#define SHADER_H

#include "glm/glm.hpp"
#include <string>

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

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath, ShaderProps props = defaultShaderProps);
    ~Shader();

    void Bind();

    void SetUniformMat4(glm::mat4, const char* name);
    void SetIntArray(const char* name, size_t size);

    int32_t GetUniformLocation(const char* name);
private:
    ShaderProps m_shaderProps;

    uint32_t m_ProgramID;
    uint32_t m_VertexID;
    uint32_t m_FragmentID;
};

#endif // SHADER_H
