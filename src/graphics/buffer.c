#include "buffer.h"

#include "gfx.h"

#include "stdlib.h"
#include "string.h"

static struct Buffer createBuffer(unsigned int type)
{
    struct Buffer self;
    glGenBuffers(1, &(self.id));
    self.data = NULL;
    self.type = type;
    self.count = 0;
    return self;
}

Vbo createVbo()
{
    return createBuffer(GL_ARRAY_BUFFER);
}

Ibo createIbo()
{
    return createBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void destroyBuffer(struct Buffer b)
{
    if (b.data != NULL)
    {
        free(b.data);
        b.data = NULL;
    }
    glDeleteBuffers(1, b.id);
}

void addDataToBuffer(struct Buffer* b, int size, const void* data)
{
#ifdef DEBUG
    b->data = malloc(size);
    memcpy(b->data, data, size);
#endif

    b->count = size / sizeof(float);

    glBindBuffer(b->type, b->id);
    glBufferData(b->type, size, data, GL_STATIC_DRAW);
}

void bindBuffer(struct Buffer b)
{
    glBindBuffer(b.type, b.id);
}