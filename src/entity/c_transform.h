#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "cglm/struct.h"

typedef struct
{
    vec3s position;
    vec3s rotation;
    vec3s scale;
}TransformComponent;

#endif