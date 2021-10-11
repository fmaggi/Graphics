#include "camera.h"

#include "log/log.h"
#include "input/input.h"

#include "input/keys.h"

Camera camera;

void orthoCamera(vec3s pos, f32t width, f32t height)
{
    camera.zoom = 1;
    camera.pos = pos;
    camera.width = width;
    camera.height = height;
    calculateViewProj();
}

int32t moveCamera(f32t xoffset, f32t yoffset)
{
    if (!isMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return 0;
        
    // glfw has its origin at the top left and I use the origin at the bottom left so I need to add one and substact the other
    camera.pos.x -= xoffset * camera.zoom;
    camera.pos.y += yoffset * camera.zoom;

    calculateViewProj();
    return 1;
}

void calculateViewProj()
{
    mat4s transform = glms_translate(glms_mat4_identity(), camera.pos);
    mat4s view = glms_mat4_inv_fast(transform);
    mat4s proj = glms_ortho(-(camera.width/2) * camera.zoom, (camera.width/2) * camera.zoom, -(camera.height/2) * camera.zoom, (camera.height/2) * camera.zoom, 0.1, 100*camera.zoom);
    camera.projview = glms_mat4_mul(proj, view);
}

void updateZoom(f32t zoom)
{
    camera.zoom -= zoom * 0.25f;
    camera.zoom = camera.zoom > 0.25f ? camera.zoom : 0.25f;
    calculateViewProj();
}

void updateProjectionMatrix(f32t width, f32t height)
{
    camera.width = width;
    camera.height = height;
    calculateViewProj();
}