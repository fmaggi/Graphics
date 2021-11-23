#ifndef VertexArray_H
#define VertexArray_H

#include "buffer.h"

class VertexArray
{
public:
    VertexArray(uint32_t stride);
    ~VertexArray();

    void Bind();
    void AddAttribute(uint32_t size);
private:
    uint32_t m_ID;
    uint32_t m_stride;
    uint32_t m_index;
    uint64_t m_offset;
};

#endif
