#ifndef VAO_H
#define VAO_H

#include "buffer.h"

struct Vao
{
    unsigned int id;
    unsigned int index;
    unsigned int offset;
};

struct Vao createVao();
void destroyVao(struct Vao vao);

void bindVao(struct Vao vao);

void addAttribute(struct Vao* vao, int size, unsigned int stride);

#endif