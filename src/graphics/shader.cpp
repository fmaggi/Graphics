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

ShaderDataType GLto_Types(uint32_t type)
{
    switch (type)
    {
    case GL_FLOAT:      return ShaderDataType::Float;
    case GL_FLOAT_VEC2: return ShaderDataType::Vec2;
    case GL_FLOAT_VEC3: return ShaderDataType::Vec3;
    case GL_FLOAT_VEC4: return ShaderDataType::Vec4;
    case GL_FLOAT_MAT2: return ShaderDataType::Mat2;
    case GL_FLOAT_MAT3: return ShaderDataType::Mat3;
    case GL_FLOAT_MAT4: return ShaderDataType::Mat4;
    }

    LOG_WARN("Invalid shader data type");
    return ShaderDataType::None;
}

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
    size_t actuallyRead = fread(src, 1, size, srcFile);
    if (actuallyRead != size)
        return 0;

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
    shaderProps = props;
    VertexID = compileShader(vertexPath, GL_VERTEX_SHADER);
    FragmentID = compileShader(fragmentPath, GL_FRAGMENT_SHADER);
    ProgramID = linkShader(VertexID, FragmentID);
    introspection();
}

Shader::~Shader()
{
    glDeleteShader(VertexID);
    glDeleteShader(FragmentID);
    glDeleteProgram(ProgramID);
    delete[] uniforms.info;
}

void Shader::Bind()
{
    glUseProgram(ProgramID);

    if (shaderProps.depthTest)
        glEnable(GL_DEPTH_TEST);

    if (shaderProps.blend != BlendFunc::None)
    {
        glEnable(GL_BLEND);
        auto [first, second] = GetGLBlendFunc(shaderProps.blend);
        glBlendFunc( first, second );
    }

    glPolygonMode(GL_FRONT_AND_BACK, GetGLFillMode(shaderProps.fill));
    CommitUniforms();
}

void Shader::Unbind()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glUseProgram(0);
}

void Shader::CommitUniforms()
{
    for (int i = 0; i < uniforms.count; i++)
    {
        UniformInfo& info = uniforms.info[i];

        switch (info.type)
        {
            case ShaderDataType::None:
                break;

            case ShaderDataType::Float:
            {
                float data = uniforms.data.ReadAs<float>(info.bytesOffset);
                glUniform1f(info.location, data);
                break;
            }
            case ShaderDataType::Vec2:
            {
                glm::vec2 data = uniforms.data.ReadAs<glm::vec2>(info.bytesOffset);
                glUniform2f(info.location, data.x, data.y);
                break;
            }
            case ShaderDataType::Vec3:
            {
                glm::vec3 data = uniforms.data.ReadAs<glm::vec3>(info.bytesOffset);
                glUniform3f(info.location, data.x, data.y, data.z);
                break;
            }
            case ShaderDataType::Vec4:
            {
                glm::vec4 data = uniforms.data.ReadAs<glm::vec4>(info.bytesOffset);
                glUniform4f(info.location, data.x, data.y, data.z, data.w);
                break;
            }
            case ShaderDataType::Mat2:
            {
                uint8_t* data = uniforms.data.ReadBytes(info.bytesOffset);
                glUniformMatrix2fv(info.location, 1, GL_FALSE, (float*)data);
                break;
            }
            case ShaderDataType::Mat3:
            {
                uint8_t* data = uniforms.data.ReadBytes(info.bytesOffset);
                glUniformMatrix3fv(info.location, 1, GL_FALSE, (float*)data);
                break;
            }
            case ShaderDataType::Mat4:
            {
                uint8_t* data = uniforms.data.ReadBytes(info.bytesOffset);
                glUniformMatrix4fv(info.location, 1, GL_FALSE, (float*)data);
                break;
            }
            default: LOG_WARN("Invalid shader data type!");
        }
    }
}

int32_t Shader::GetUniformLocation(const char* name)
{
    int location = glGetUniformLocation(ProgramID, name);
    if (location == -1)
        LOG_WARN("Invalid uniform: %s", name);
    return location;
}

UniformInfo Shader::GetUniformInfo(const std::string& name)
{
    for (uint32_t i = 0; i < uniforms.count; i++)
    {
        if (name == uniforms.info[i].name)
            return uniforms.info[i];
    }
    return UniformInfo{};
}

void Shader::introspection()
{
    constexpr uint8_t block = 0;
    constexpr uint8_t t = 1;
    constexpr uint8_t nameL = 2;
    constexpr uint8_t loc = 3;

    uint32_t current = 0;

    GLint numUniforms = 0;
    glGetProgramInterfaceiv(ProgramID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
    uniforms.count = numUniforms;
    uniforms.info = new UniformInfo[numUniforms];

    const GLenum properties[4] = {GL_BLOCK_INDEX, GL_TYPE, GL_NAME_LENGTH, GL_LOCATION};

    uint32_t uniformsTotalSize = 0;

    for(int unif = 0; unif < numUniforms; ++unif)
    {
        GLint values[4];
        glGetProgramResourceiv(ProgramID, GL_UNIFORM, unif, 4, properties, 4, NULL, values);
        // Skip any uniforms that are in a block or samplers.
        if(values[block] != -1 || values[t] == GL_SAMPLER_2D)
        {
            uniforms.count--; // This might lead to a bit of wasted mem. But as long as there arent too many samplers or block in the shader its fine
            continue;
        }

        int nameLength = values[nameL];

        std::string nameData;
        nameData.resize(nameLength);
        glGetProgramResourceName(ProgramID, GL_UNIFORM, unif, nameLength, NULL, &nameData[0]);

        UniformInfo& info = uniforms.info[current++];

        info.name = std::string(nameData.begin(), nameData.end() - 1);
        info.location = values[loc];

        ShaderDataType type = GLto_Types(values[t]);
        info.type = type;

        uint32_t uniformSize = ShaderTypeSize(type);
        info.size = uniformSize;
        info.bytesOffset = uniformsTotalSize;

        uniformsTotalSize += uniformSize;
    }

    uniforms.data.Resize(uniformsTotalSize);
}

