#include "camera.h"

#include "log/log.h"

#include "glm/gtc/matrix_transform.hpp"

Camera::Camera()
{
    zoom = 100.0f;
    translation = {0.0f, 0.0f, 0.0f};
    width = 0;
    height = 0;
    viewproj = glm::mat4(1.0f);
}

void Camera::Move(glm::vec2 offset)
{
    translation += glm::vec3(offset, 0.0f) / zoom;
    CalculateViewProj();
}

void Camera::CalculateViewProj()
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 view = glm::inverse(transform);
    glm::mat4 proj = glm::ortho(-(width/2) / zoom, (width/2) / zoom, -(height/2) / zoom, (height/2) / zoom);
    viewproj = proj * view;
}

void Camera::Zoom(float zoom_)
{
    zoom += zoom_ * 10;
    zoom = std::max(0.25f, zoom);
    CalculateViewProj();
}

void Camera::SetWidthAndHeight(float width_, float height_)
{
    width = width_;
    height = height_;
    CalculateViewProj();
}

bool Camera::InFrustum(float left, float right, float top, float bottom)
{
    bool sideToSide = right > (-width/2) / zoom + translation.x
                    && left < (width/2) / zoom + translation.x;

    bool topToBottom = top > (-height/2) / zoom + translation.y
                    && bottom < (height/2) / zoom  + translation.y;

    return sideToSide && topToBottom;
}
