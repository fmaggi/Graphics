#include "camera.h"

#include "log/log.h"
#include "game/input.h"

#include "glm/gtc/matrix_transform.hpp"

Camera camera;

void orthoCamera(glm::vec3 translation, float width, float height)
{
    camera.zoom = 100.0f;
    camera.translation = translation;
    camera.width = width;
    camera.height = height;
    calculateViewProj();
}

void moveCamera(float xoffset, float yoffset)
{
    camera.translation.x += xoffset / camera.zoom;
    camera.translation.y += yoffset / camera.zoom;
    calculateViewProj();
}

void calculateViewProj()
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), camera.translation);
    glm::mat4 view = glm::inverse(transform);
    glm::mat4 proj = glm::ortho(-(camera.width/2) / camera.zoom, (camera.width/2) / camera.zoom, -(camera.height/2) / camera.zoom, (camera.height/2) / camera.zoom);
    camera.projview = proj * view;
}

void updateZoom(float zoom)
{
    camera.zoom += zoom;
    camera.zoom = std::max(0.25f, camera.zoom);
    calculateViewProj();
}

void updateProjectionMatrix(float width, float height)
{
    camera.width = width;
    camera.height = height;
    calculateViewProj();
}

bool inFrustum(float left, float right, float top, float bottom)
{
    bool sideToSide = right > (-camera.width/2) / camera.zoom + camera.translation.x
                    && left < (camera.width/2) / camera.zoom + camera.translation.x;

    bool topToBottom = top > (-camera.height/2) / camera.zoom + camera.translation.y
                    && bottom < (camera.height/2) / camera.zoom  + camera.translation.y;

    return sideToSide && topToBottom;
}
