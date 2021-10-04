#ifndef ENTITY_H
#define ENTITY_H

#include "graphics/vao.h"
#include "graphics/buffer.h"

typedef struct _entity
{
    struct Vao vao;
    Vbo vbo;
    Ibo ibo;
    int pos;
} Entity;

#endif