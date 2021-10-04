#ifndef ENTITY_H
#define ENTITY_H

#include "graphics/vao.h"
#include "graphics/buffer.h"

#include "cglm/struct.h"

typedef struct _entity // basically just a mesh
{
    struct Vao vao;
    Vbo vbo;
    Ibo ibo;
    vec3s pos;
} Entity;

#endif