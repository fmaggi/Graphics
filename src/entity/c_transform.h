#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "cglm/struct.h"

typedef struct
{
    vec3s position;
    float rotation;
    vec2s scale;
}TransformComponent;

#endif