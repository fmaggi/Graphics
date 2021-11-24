#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"

struct Camera
{
    glm::vec3 translation;
    glm::mat4 projview;
    float width, height, zoom;
};

extern Camera camera;

bool inFrustum(float left, float right, float top, float bottom);

void orthoCamera(glm::vec3 pos, float width, float height);

void moveCamera(float xpos, float ypos);
void updateZoom(float zoom);
void calculateViewProj();
void updateProjectionMatrix(float width, float height);

#endif
