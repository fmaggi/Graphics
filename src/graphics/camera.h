#ifndef CAMERA_H
#define CAMERA_H

#include "cglm/struct.h"

typedef struct _camera
{
    vec3s pos;
    mat4s proj;
    mat4s view;
    mat4s projview;
    float width, height, zoom;
} Camera;

Camera orthoCamera(vec3s pos, float width, float height);

int moveCamera(Camera* c, float xpos, float ypos);

void updateZoom(Camera* c, float zoom);
void updateViewMatrix(Camera* c);
void updateProjectionMatrix(Camera* c, float width, float height);

#endif