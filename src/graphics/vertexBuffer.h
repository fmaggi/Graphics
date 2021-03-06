#ifndef V_BUFFER_H
#define V_BUFFER_H

#include "stdint.h"

struct VertexBuffer
{
    enum class BufferType
    {
        Static = 0, Dynamic
    };
    VertexBuffer(uint32_t vertexSize, uint32_t vertexCount, BufferType type = BufferType::Dynamic, const void* data = 0);
    ~VertexBuffer();

    void Bind();
    void PushData(uint32_t size, const void* data);

    void AddAttribute(uint32_t size);
    const BufferType m_type;
private:
    uint32_t m_ID;

    uint32_t m_stride;
    uint32_t m_index;
    uint64_t m_offset;
};

#endif
