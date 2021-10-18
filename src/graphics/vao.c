#include "vao.h"

#include "gfx.h"

#include "log/log.h"

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

void addAttribute(Vao* vao,  int size, uint32_t stride)
{
    uint32_t index = vao->index;
    unsigned long int offset = vao->offset; // the long is just to remove some warngins

    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    glEnableVertexAttribArray(index);

    index += 1;
    vao->index = index;
    vao->offset += size * sizeof(float);

    LOG_INFO_DEBUG("Vao layout: element %i offset =, %li\n", index, offset);
}