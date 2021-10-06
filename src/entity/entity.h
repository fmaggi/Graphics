#ifndef ENTITY_H
#define ENTITY_H

#include "cglm/struct.h"

typedef struct _entity // basically just a mesh
{
    vec3s pos;
    vec3s color;
} Entity;

#endif