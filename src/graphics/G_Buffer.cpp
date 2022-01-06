#include "vertexBuffer.h"
#include "indexBuffer.h"

#include "gfx.h"

#include "log/log.h"

static inline GLenum GetGLType(VertexBuffer::BufferType type)
{
    switch (type)
    {
        case VertexBuffer::BufferType::Static:  return GL_STATIC_DRAW;
        case VertexBuffer::BufferType::Dynamic: return GL_DYNAMIC_DRAW;
    }
    LOG_WARN("Invalid buffer type");
    return GL_STATIC_DRAW;
}

VertexBuffer::VertexBuffer(uint32_t vertexSize, uint32_t vertexCount, BufferType type, const void* data)
    : m_type(type)
{
    glGenBuffers(1, &m_ID);

    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, vertexSize * vertexCount, data, GetGLType(type));

    m_stride = vertexSize;
    m_offset = 0;
    m_index = 0;
};

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_ID);
}

void VertexBuffer::PushData(uint32_t size, const void* data)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VertexBuffer::AddAttribute(uint32_t size)
{
    glVertexAttribPointer(m_index, size, GL_FLOAT, GL_FALSE, m_stride, (void*)m_offset);
    glEnableVertexAttribArray(m_index);

    m_index++;
    m_offset += size * sizeof(float);
}

IndexBuffer::IndexBuffer(uint32_t count, uint32_t* data)
    : vertexCount(count)
{
    glGenBuffers(1, &ID);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &ID);
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}
