#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"

class Camera
{
public:
    Camera();

    void Move(glm::vec2 offset);
    void Zoom(float zoom);
    void SetWidthAndHeight(float width, float height);

    bool InFrustum(float left, float right, float top, float bottom);

    inline const glm::mat4& GetViewProjMatrix() const { return m_viewproj; }

private:
    void CalculateViewProj();

    glm::vec3 m_translation;
    glm::mat4 m_viewproj;
    float m_width, m_height, m_zoom;
};

#endif
