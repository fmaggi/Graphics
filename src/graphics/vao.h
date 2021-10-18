#ifndef VAO_H
#define VAO_H

#include "buffer.h"

typedef struct vao
{
    uint32_t id;
    uint32_t index;
    uint32_t offset;
} Vao;

Vao createVao();
void destroyVao(Vao vao);

void bindVao(Vao vao);

void addAttribute(Vao* vao, int size, uint32_t stride);

#endif