#ifndef BUFFER_H
#define BUFFER_H

#include "stdint.h"

class VertexBuffer
{
public:
    enum class BufferType
    {
        Static = 0, Dynamic
    };
    VertexBuffer(uint32_t size, BufferType type = BufferType::Dynamic, const void* data = 0);
    ~VertexBuffer();

    void Bind();
    void PushData(uint32_t size, const void* data);
private:
    uint32_t m_ID;
    BufferType m_type;
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
