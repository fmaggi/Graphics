#ifndef UNIFORMS_H
#define UNIFORMS_H

#include <string>
#include <unordered_map>

#include "shaderData.h"

#include "base/buffer.h"

// Default is invalid uniform
struct UniformInfo
{
    std::string name;
    int32_t location = -1;
    ShaderDataType type = ShaderDataType::None;
    uint32_t size = 0;
    uint32_t bytesOffset = 0;
};

struct Uniforms
{
    UniformInfo* info = nullptr;
    Buffer data;
    uint32_t count = 0;
};


#endif
