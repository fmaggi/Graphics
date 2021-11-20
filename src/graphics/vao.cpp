#include "vao.h"

#include "gfx.h"

#include "log/log.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_ID);
    glBindVertexArray(m_ID);

    m_index = 0;
    m_offset = 0;
    m_stride = 0;
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::Bind()
{
    glBindVertexArray(m_ID);
}

void VertexArray::AddAttribute(uint32_t size)
{
    ASSERT(m_stride > 0, "Need to set stride");

    glBindVertexArray(m_ID);

    glVertexAttribPointer(m_index, size, GL_FLOAT, GL_FALSE, m_stride, (void*)m_offset);
    glEnableVertexAttribArray(m_index);

    m_index++;
    m_offset += size * sizeof(float);
}
