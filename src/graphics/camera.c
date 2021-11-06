#include "camera.h"

#include "log/log.h"
#include "game/input.h"

Camera camera;

void orthoCamera(vec3s pos, float width, float height)
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
    mat4s transform = glms_translate(glms_mat4_identity(), camera.pos);
    mat4s view = glms_mat4_inv_fast(transform);
    mat4s proj = glms_ortho(-(camera.width/2) * camera.zoom, (camera.width/2) * camera.zoom, -(camera.height/2) * camera.zoom, (camera.height/2) * camera.zoom, 0.1, 100*camera.zoom);
    camera.projview = glms_mat4_mul(proj, view);
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
