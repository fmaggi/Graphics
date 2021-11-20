#include "buffer.h"

#include "gfx.h"

#include "stdlib.h"
#include "string.h"

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

VertexBuffer::VertexBuffer(uint32_t size, BufferType type, const void* data)
{
    glGenBuffers(1, &m_ID);

    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GetGLType(type));

    m_type = type;
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

IndexBuffer::IndexBuffer(uint32_t count, uint32_t* data)
{
    glGenBuffers(1, &m_ID);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);

    m_vertexCount = count;
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_ID);
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}
