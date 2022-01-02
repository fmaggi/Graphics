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

size_t ShaderTypeSize(ShaderDataType type);

template<typename T> constexpr ShaderDataType shaderDataType()  { return ShaderDataType::None;  }

template<> constexpr ShaderDataType shaderDataType<float>()     { return ShaderDataType::Float; }
template<> constexpr ShaderDataType shaderDataType<glm::vec2>() { return ShaderDataType::Vec2;  }
template<> constexpr ShaderDataType shaderDataType<glm::vec3>() { return ShaderDataType::Vec3;  }
template<> constexpr ShaderDataType shaderDataType<glm::vec4>() { return ShaderDataType::Vec4;  }
template<> constexpr ShaderDataType shaderDataType<glm::mat2>() { return ShaderDataType::Mat2;  }
template<> constexpr ShaderDataType shaderDataType<glm::mat3>() { return ShaderDataType::Mat3;  }
template<> constexpr ShaderDataType shaderDataType<glm::mat4>() { return ShaderDataType::Mat4;  }

#endif
