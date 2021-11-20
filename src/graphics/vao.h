#ifndef VertexArray_H
#define VertexArray_H

#include "buffer.h"

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Bind();
    void AddAttribute(uint32_t size);

    inline void SetStride(uint32_t stride) { m_stride = stride; }
private:
    uint32_t m_ID;
    uint32_t m_stride;
    uint32_t m_index;
    uint64_t m_offset;
};

#endif
