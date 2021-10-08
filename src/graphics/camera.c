#include "camera.h"

#include "log/log.h"
#include "graphics/window.h"

#include "input/keys.h"

Camera camera;

void orthoCamera(vec3s pos, float width, float height)
{
    camera.zoom = 1;
    camera.pos = pos;
    camera.width = width;
    camera.height = height;
    camera.proj = glms_ortho(-(camera.width/2) * camera.zoom, (camera.width/2) * camera.zoom, -(camera.height/2) * camera.zoom, (camera.height/2), -camera.zoom, camera.zoom);
    updateViewMatrix();
}

int moveCamera(float xoffset, float yoffset)
{
    if (!isMouseButtonPressed(MOUSE_BUTTON_1))
        return 0;
        
    // glfw has its origin at the top left and I use the origin at the bottom left so I need to add one and substact the other
    camera.pos.x -= xoffset * camera.zoom;
    camera.pos.y += yoffset * camera.zoom;

    updateViewMatrix();
    return 1;
}

void updateViewMatrix()
{
    mat4s transform = glms_translate(glms_mat4_identity(), camera.pos);
    mat4s view = glms_mat4_inv_fast(transform);
    camera.view = (view);
    camera.projview = glms_mat4_mul(camera.proj, camera.view);
}

void updateZoom(float zoom)
{
    camera.zoom -= zoom * 0.25f;
    camera.zoom = camera.zoom > 0.25f ? camera.zoom : 0.25f;
    camera.proj = glms_ortho(-(camera.width/2) * camera.zoom, (camera.width/2) * camera.zoom, -(camera.height/2) * camera.zoom, (camera.height/2) * camera.zoom, -camera.zoom, camera.zoom);
    camera.projview = glms_mat4_mul(camera.proj, camera.view);
}

void updateProjectionMatrix(float width, float height)
{
    camera.width = width;
    camera.height = height;
    camera.proj = glms_ortho(-(camera.width/2) * camera.zoom, (camera.width/2) * camera.zoom, -(camera.height/2) * camera.zoom, (camera.height/2) * camera.zoom, -camera.zoom, camera.zoom);
    camera.projview = glms_mat4_mul(camera.proj, camera.view);
}