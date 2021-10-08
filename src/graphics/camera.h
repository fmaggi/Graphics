#ifndef CAMERA_H
#define CAMERA_H

#include "cglm/struct.h"

typedef struct _camera
{
    vec3s pos;
    mat4s projview;
    float width, height, zoom;
} Camera;

extern Camera camera;

void orthoCamera(vec3s pos, float width, float height);

int moveCamera(float xpos, float ypos);

void updateZoom(float zoom);
void calculateViewProj();
void updateProjectionMatrix(float width, float height);

#endif