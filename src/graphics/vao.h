#ifndef VAO_H
#define VAO_H

#include "buffer.h"

typedef struct vao
{
    unsigned int id;
    unsigned int index;
    unsigned int offset;
} Vao;

Vao createVao();
void destroyVao(Vao vao);

void bindVao(Vao vao);

void addAttribute(Vao* vao, int size, unsigned int stride);

#endif