#ifndef G_BUFFER_H
#define G_BUFFER_H

#include "stdint.h"

class VertexBuffer
{
public:
    enum class BufferType
    {
        Static = 0, Dynamic
    };
    VertexBuffer(uint32_t vertexSize, uint32_t vertexCount, BufferType type = BufferType::Dynamic, const void* data = 0);
    ~VertexBuffer();

    void Bind();
    void PushData(uint32_t size, const void* data);

    void AddAttribute(uint32_t size);
private:
    uint32_t m_ID;
    BufferType m_type;

    uint32_t m_stride;
    uint32_t m_index;
    uint64_t m_offset;
};

class IndexBuffer
{
public:
    IndexBuffer(uint32_t count, uint32_t* data);
    ~IndexBuffer();

    void Bind();
private:
    uint32_t m_ID;
    uint32_t m_vertexCount;
};

#endif
