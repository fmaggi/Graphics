#include "buffer.h"

#include "gfx.h"

#include "stdlib.h"
#include "string.h"

Vbo createVbo(unsigned int size)
{
    struct Buffer self;
    glGenBuffers(1, &self.id);
    
    glBindBuffer(GL_ARRAY_BUFFER, self.id);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);

    self.type = GL_ARRAY_BUFFER;

    return self;
}

Vbo createStaticVbo(unsigned int size, const void* data)
{
    struct Buffer self;
    glGenBuffers(1, &self.id);
    
    glBindBuffer(GL_ARRAY_BUFFER, self.id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    self.type = GL_ARRAY_BUFFER;

    return self;
}

Ibo createIbo(unsigned int count, unsigned int * data)
{
    struct Buffer self;
    glGenBuffers(1, &self.id);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self.id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);

    self.type = GL_ELEMENT_ARRAY_BUFFER;
    self.count = count;

    return self;
}

void destroyBuffer(struct Buffer b)
{
    glDeleteBuffers(1, &b.id);
}

void pushBufferData(struct Buffer b, int size, const void* data)
{
    glBindBuffer(b.type, b.id);
    glBufferSubData(b.type, 0, size, data);
}

void bindBuffer(struct Buffer b)
{
    glBindBuffer(b.type, b.id);
}