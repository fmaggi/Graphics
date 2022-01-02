#ifndef SHADER_DATA_H
#define SHADER_DATA_H

#include "glm/glm.hpp"

enum class ShaderDataType
{
    None = 0,

    Float,
    Vec2,
    Vec3,
    Vec4,
    Mat2,
    Mat3,
    Mat4
};

// Default is invalid uniform
struct UniformInfo
{
    int32_t location = -1;
    ShaderDataType type = ShaderDataType::None;
    uint32_t size = 0;
    uint32_t bytesOffset = 0;
};

size_t ShaderTypeSize(ShaderDataType type);

template<typename T> constexpr ShaderDataType shaderType()  { return ShaderDataType::None;  }

template<> constexpr ShaderDataType shaderType<float>()     { return ShaderDataType::Float; }
template<> constexpr ShaderDataType shaderType<glm::vec2>() { return ShaderDataType::Vec2;  }
template<> constexpr ShaderDataType shaderType<glm::vec3>() { return ShaderDataType::Vec3;  }
template<> constexpr ShaderDataType shaderType<glm::vec4>() { return ShaderDataType::Vec4;  }
template<> constexpr ShaderDataType shaderType<glm::mat2>() { return ShaderDataType::Mat2;  }
template<> constexpr ShaderDataType shaderType<glm::mat3>() { return ShaderDataType::Mat3;  }
template<> constexpr ShaderDataType shaderType<glm::mat4>() { return ShaderDataType::Mat4;  }

#endif
