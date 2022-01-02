#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

#include "shader.h"
#include "glm/glm.hpp"

#include "base/buffer.h"

class Material
{
public:

    glm::vec4 albedo{ 1.0f, 1.0f, 1.0f, 1.0f };
    std::string name = "NoName Material";

public:
    Material() = default;
    Material(Shader* shader);

    void SetShader(Shader* shader);

    template<typename T>
    bool Set(const std::string& name, T& value);

    void Bind();
private:

    void LoadToShader();
    void GetShaderUniformsInfo();

    Shader* m_shader = nullptr;
    Buffer m_uniforms;
};

template<typename T>
bool Material::Set(const std::string& uniformName, T& value)
{
    // TODO: Implement
    UniformInfo info = m_shader->GetUniformInfo(uniformName);
    if (info.location == -1)
        return false;

    m_uniforms.Write(value, info.bytesOffset);
    return true;
}

#endif
