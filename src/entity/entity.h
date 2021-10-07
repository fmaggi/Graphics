#ifndef ENTITY_H
#define ENTITY_H

#include "cglm/struct.h"

typedef struct _entity
{
    vec3s pos;
    vec3s color;
    vec2s speed;
} Entity;

#endif