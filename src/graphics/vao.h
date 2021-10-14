#ifndef VAO_H
#define VAO_H

#include "buffer.h"

typedef struct vao
{
    uint32t id;
    uint32t index;
    uint32t offset;
} Vao;

Vao createVao();
void destroyVao(Vao vao);

void bindVao(Vao vao);

void addAttribute(Vao* vao, int32t size, uint32t stride);

#endif