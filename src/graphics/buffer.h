#ifndef BUFFER_H
#define BUFFER_H

struct Buffer
{
    unsigned int type;
    unsigned int id;
    unsigned int count;
};

typedef struct Buffer Vbo;
typedef struct Buffer Ibo;

Vbo createVbo(unsigned int size);
Vbo createStaticVbo(unsigned int size, const void* data);
Ibo createIbo(unsigned int count, unsigned int* data);

void destroyBuffer(struct Buffer b);

void pushBufferData(struct Buffer b, int size, const void* data);
void bindBuffer(struct Buffer b);

#endif