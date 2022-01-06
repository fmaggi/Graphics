#include "vertexArray.h"

#include "gfx.h"

#include "log/log.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &ID);
    glBindVertexArray(ID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &ID);
}

void VertexArray::Bind()
{
    glBindVertexArray(ID);
}
