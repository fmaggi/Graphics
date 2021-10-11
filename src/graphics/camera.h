#ifndef CAMERA_H
#define CAMERA_H

#include "cglm/struct.h"

#include "util/types.h"

typedef struct camera
{
    vec3s pos;
    mat4s projview;
    f32t width, height, zoom;
} Camera;

extern Camera camera;

void orthoCamera(vec3s pos, f32t width, f32t height);

int moveCamera(f32t xpos, f32t ypos);

void updateZoom(f32t zoom);
void calculateViewProj();
void updateProjectionMatrix(f32t width, f32t height);

#endif