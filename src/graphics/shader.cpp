#include "shader.h"

#include "gfx.h"

#include "log/log.h"
#include "stdlib.h"
#include "string.h"

#include <tuple>

ShaderProps defaultShaderProps = {
    .blend = BlendFunc::Aplha_Src_OneMinusSrc,
    .depthTest = true,
    .fill = FillMode::Fill
};

size_t ShaderTypeSize(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::None:  return 0;
    case ShaderDataType::Float: return sizeof(float);
    case ShaderDataType::Vec2:  return sizeof(float) * 2;
    case ShaderDataType::Vec3:  return sizeof(float) * 3;
    case ShaderDataType::Vec4:  return sizeof(float) * 4;
    case ShaderDataType::Mat2:  return sizeof(float) * 2 * 2;
    case ShaderDataType::Mat3:  return sizeof(float) * 3 * 3;
    case ShaderDataType::Mat4:  return sizeof(float) * 4 * 4;
    }

    LOG_WARN("Invalid shader data type");
    return 0;
}

static uint32_t compileShader(const char* path, uint32_t type)
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
    char* src = (char*)malloc(size * sizeof(char) + 1);
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
        const char* s = type == GL_VERTEX_SHADER ? "Vertex" : "Fragment";
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        LOG_ERROR("%s shader compilation:", s);
        LOG("  %s", infoLog);
        exit(-1);
    }
    return shader;
}

static uint32_t linkShader(uint32_t vertexID, uint32_t fragmentID)
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

static auto constexpr GetGLBlendFunc(BlendFunc func)
{
    switch (func)
    {
    case BlendFunc::None: return std::tuple(0, 0);
    case BlendFunc::Aplha_Src_OneMinusSrc: return std::tuple(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    LOG_WARN("Invalid Blend function!");
    return std::tuple(0, 0);
}

static auto constexpr GetGLFillMode(FillMode mode)
{
    switch (mode)
    {
    case FillMode::Fill: return GL_FILL;
    case FillMode::Line: return GL_LINE;
    }

    LOG_WARN("Invalid Blend function!");
    return GL_FILL;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, ShaderProps props)
{
    m_shaderProps = props;
    m_VertexID = compileShader(vertexPath, GL_VERTEX_SHADER);
    m_FragmentID = compileShader(fragmentPath, GL_FRAGMENT_SHADER);
    m_ProgramID = linkShader(m_VertexID, m_FragmentID);
}

Shader::~Shader()
{
    glDeleteShader(m_VertexID);
    glDeleteShader(m_FragmentID);
    glDeleteProgram(m_ProgramID);
}

void Shader::Bind()
{
    glUseProgram(m_ProgramID);

    if (m_shaderProps.depthTest)
        glEnable(GL_DEPTH_TEST);

    if (m_shaderProps.blend != BlendFunc::None)
    {
        glEnable(GL_BLEND);
        auto [first, second] = GetGLBlendFunc(m_shaderProps.blend);
        glBlendFunc( first, second );
    }

    glPolygonMode(GL_FRONT_AND_BACK, GetGLFillMode(m_shaderProps.fill));
}

void Shader::Unbind()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glUseProgram(0);
}

int32_t Shader::GetUniformLocation(const char* name)
{
    int location = glGetUniformLocation(m_ProgramID, name);
    if (location == -1)
        LOG_WARN("Invalid uniform: %s", name);
    return location;
}

void Shader::SetUniformMat4(glm::mat4 mat, const char* name)
{
    glUseProgram(m_ProgramID);
    int location = GetUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, (float *)&mat[0][0]);
}

void Shader::SetIntArray(const char* name, size_t size)
{
    glUseProgram(m_ProgramID);
    int location = GetUniformLocation(name);
    GLint slots[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    glUniform1iv(location, 16, slots);
}

UniformInfo Shader::GetUniformInfo(const std::string& name)
{
    auto it = m_uniforms.find(name);
    if (it == m_uniforms.end())
        return UniformInfo{};

    return m_uniforms[name];
}

uint32_t Shader::Tell()
{
    return m_uniforms.size();
}

