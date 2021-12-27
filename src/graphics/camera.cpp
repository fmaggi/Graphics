#include "camera.h"

#include "log/log.h"

#include "glm/gtc/matrix_transform.hpp"

Camera::Camera()
{
    m_zoom = 100.0f;
    m_translation = {0.0f, 0.0f, 0.0f};
    m_width = 0;
    m_height = 0;
    m_viewproj = glm::mat4(1.0f);
}

void Camera::Move(glm::vec2 offset)
{
    m_translation += glm::vec3(offset, 0.0f) / m_zoom;
    CalculateViewProj();
}

void Camera::CalculateViewProj()
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_translation);
    glm::mat4 view = glm::inverse(transform);
    glm::mat4 proj = glm::ortho(-(m_width/2) / m_zoom, (m_width/2) / m_zoom, -(m_height/2) / m_zoom, (m_height/2) / m_zoom);
    m_viewproj = proj * view;
}

void Camera::Zoom(float zoom)
{
    m_zoom += zoom;
    m_zoom = std::max(0.25f, m_zoom);
    CalculateViewProj();
}

void Camera::SetWidthAndHeight(float width, float height)
{
    m_width = width;
    m_height = height;
    CalculateViewProj();
}

bool Camera::InFrustum(float left, float right, float top, float bottom)
{
    bool sideToSide = right > (-m_width/2) / m_zoom + m_translation.x
                    && left < (m_width/2) / m_zoom + m_translation.x;

    bool topToBottom = top > (-m_height/2) / m_zoom + m_translation.y
                    && bottom < (m_height/2) / m_zoom  + m_translation.y;

    return sideToSide && topToBottom;
}
