#ifndef SPRITE_H
#define SPRITE_H

#include "cglm/struct.h"

typedef struct
{
    vec3s color;
    float texIndex;
    int render; // this is just a hack
} SpriteComponent;

#endif