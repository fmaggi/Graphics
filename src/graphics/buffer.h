#ifndef BUFFER_H
#define BUFFER_H

#include "util/types.h"

struct Buffer
{
    uint32t type;
    uint32t id;
    uint32t count;
};

typedef struct Buffer Vbo;
typedef struct Buffer Ibo;

Vbo createVbo(uint32t size);
Vbo createStaticVbo(uint32t size, const void* data);
Ibo createIbo(uint32t count, uint32t* data);

void destroyBuffer(struct Buffer b);

void pushBufferData(struct Buffer b, int32t size, const void* data);
void bindBuffer(struct Buffer b);

#endif