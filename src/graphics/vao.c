#include "vao.h"

#include "gfx.h"
#include "stdio.h"

Vao createVao()
{
    Vao self;
    glGenVertexArrays(1, &(self.id));

    self.index = 0;
    self.offset = 0;

    return self;
}

void destroyVao(Vao vao)
{
    glDeleteVertexArrays(1, &(vao.id));
}

void bindVao(Vao vao)
{
    glBindVertexArray(vao.id);
}

void addAttribute(Vao* vao,  int size, unsigned int stride)
{
    unsigned int index = vao->index;
    unsigned long int offset = vao->offset; // the long is just to remove some warngins

    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    glEnableVertexAttribArray(index);

    index += 1;
    vao->index = index;
    vao->offset += size * sizeof(float) * index;
}