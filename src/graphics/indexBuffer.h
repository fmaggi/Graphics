#ifndef I_BUFFER_H
#define I_BUFFER_H

#include "stdint.h"

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
