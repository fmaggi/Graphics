#include "vertexArray.h"

#include "gfx.h"

#include "log/log.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_ID);
    glBindVertexArray(m_ID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::Bind()
{
    glBindVertexArray(m_ID);
}
