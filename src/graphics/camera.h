#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"

struct Camera
{
    Camera();

    void Move(glm::vec2 offset);
    void Zoom(float zoom);
    void SetWidthAndHeight(float width, float height);

    bool InFrustum(float left, float right, float top, float bottom);
    void CalculateViewProj();

    glm::vec3 translation;
    glm::mat4 viewproj;
    float width, height, zoom;
};

#endif
