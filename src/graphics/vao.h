#ifndef VertexArray_H
#define VertexArray_H

#include "buffer.h"

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Bind();
private:
    uint32_t m_ID;
};

#endif
