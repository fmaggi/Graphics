#ifndef VertexArray_H
#define VertexArray_H

typedef unsigned int uint32_t;

#include "vertexBuffer.h"
#include "indexBuffer.h"

struct VertexArray
{
    VertexArray();
    ~VertexArray();

    void Bind();
    uint32_t ID;
};

#endif
