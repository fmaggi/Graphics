#include "camera.h"

#include "log/log.h"
#include "game/input.h"

#include "glm/gtc/matrix_transform.hpp"

Camera camera;

void orthoCamera(glm::vec3 pos, float width, float height)
{
    camera.zoom = 1;
    camera.pos = pos;
    camera.width = width;
    camera.height = height;
    calculateViewProj();
}

void moveCamera(float xoffset, float yoffset)
{
    // to move it with mouse x axis movement needs to be negative because of different origin placement by GLFW and OpenGL
    camera.pos.x += xoffset * camera.zoom;
    camera.pos.y += yoffset * camera.zoom;
    calculateViewProj();
}

void calculateViewProj()
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), camera.pos);
    glm::mat4 view = glm::inverse(transform);
    glm::mat4 proj = glm::ortho(-(camera.width/2) * camera.zoom, (camera.width/2) * camera.zoom, -(camera.height/2) * camera.zoom, (camera.height/2) * camera.zoom);
    camera.projview = proj * view;
}

void updateZoom(float zoom)
{
    camera.zoom -= zoom * 0.25f;
    camera.zoom = camera.zoom > 0.25f ? camera.zoom : 0.25f;
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
    bool sideToSide = left > (-camera.width/2) * camera.zoom + camera.pos.x
                    && right < (camera.width/2) * camera.zoom + camera.pos.x;

    bool topToBottom = top > (-camera.height/2) * camera.zoom + camera.pos.y
                    && bottom < (camera.height/2) * camera.zoom  + camera.pos.y;

    return sideToSide && topToBottom;
}