#ifndef I_BUFFER_H
#define I_BUFFER_H

#include "stdint.h"

struct IndexBuffer
{
    IndexBuffer(uint32_t count, uint32_t* data);
    ~IndexBuffer();

    void Bind();

    const uint32_t vertexCount;
private:
    uint32_t ID;
};


#endif
