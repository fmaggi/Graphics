#ifndef BUFFER_H
#define BUFFER_H

#include "stdint.h"

struct Buffer
{
    uint32_t type;
    uint32_t id;
    uint32_t count;
};

typedef struct Buffer Vbo;
typedef struct Buffer Ibo;

Vbo createVbo(uint32_t size);
Vbo createStaticVbo(uint32_t size, const void* data);
Ibo createIbo(uint32_t count, uint32_t* data);

void destroyBuffer(struct Buffer b);

void pushBufferData(struct Buffer b, int size, const void* data);
void bindBuffer(struct Buffer b);

#endif