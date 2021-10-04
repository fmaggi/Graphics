#ifndef BUFFER_H
#define BUFFER_H

struct Buffer
{
    unsigned int type;
    unsigned int id;
    unsigned int count;
    void* data;
};

typedef struct Buffer Vbo;
typedef struct Buffer Ibo;

Vbo createVbo();
Ibo createIbo();
void destroyBuffer(struct Buffer* b);

void addDataToBuffer(struct Buffer* b, int size, const void* data);
void bindBuffer(struct Buffer b);

#endif