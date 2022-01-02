#include "material.h"

#include "log/log.h"

Material::Material(Shader* shader)
    : m_shader(shader)
{
    GetShaderUniformsInfo();
}

void Material::SetShader(Shader* shader)
{
    m_shader = shader;
    GetShaderUniformsInfo();
}

void Material::Bind()
{
    if (!m_shader)
    {
        LOG_WARN("Material %s doesn't have a shader attached!", name.c_str());
        return;
    }
    m_shader->Bind();
    LoadToShader();
}

void Material::LoadToShader()
{
    // TODO: Implement
}

void Material::GetShaderUniformsInfo()
{
    // TODO: Implement

    /*

    uint32_t size = m_shader->Tell();
    m_unfiroms.Resize(size);

    */
}
