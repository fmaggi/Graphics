#ifndef UNIFORMS_H
#define UNIFORMS_H

#include <string>
#include <unordered_map>

#include "shaderData.h"

#include "base/buffer.h"

struct Uniforms
{
    UniformInfo* info = nullptr;
    Buffer data;
    uint32_t count = 0;
};


#endif
