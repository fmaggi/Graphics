#include "camera.h"

#include "log/log.h"
#include "graphics/window.h"

#include "input/keys.h"

Camera orthoCamera(vec3s pos, float width, float height)
{
    Camera self;
    self.zoom = 1;
    self.pos = pos;
    self.width = width;
    self.height = height;
    self.proj = glms_ortho(-(self.width/2) * self.zoom, (self.width/2) * self.zoom, -(self.height/2) * self.zoom, (self.height/2), -self.zoom, self.zoom);
    updateViewMatrix(&self);
    return self;
}

int moveCamera(Camera* self, float xoffset, float yoffset)
{
    if (!isMouseButtonPressed(MOUSE_BUTTON_1))
        return 0;
        

    // glfw has its origin at the top left and I use the origin at the bottom left so I need to add one and substact the other
    self->pos.x -= xoffset * self->zoom;
    self->pos.y += yoffset * self->zoom;

    updateViewMatrix(self);
    return 1;
}

void updateViewMatrix(Camera* self)
{
    mat4s transform = glms_translate(glms_mat4_identity(), self->pos);
    mat4s view = glms_mat4_inv_fast(transform);
    self->view = (view);
    self->projview = glms_mat4_mul(self->proj, self->view);
}

void updateZoom(Camera* self, float zoom)
{
    self->zoom -= zoom * 0.25f;
    self->zoom = self->zoom > 0.25f ? self->zoom : 0.25f;
    self->proj = glms_ortho(-(self->width/2) * self->zoom, (self->width/2) * self->zoom, -(self->height/2) * self->zoom, (self->height/2) * self->zoom, -self->zoom, self->zoom);
    self->projview = glms_mat4_mul(self->proj, self->view);
}

void updateProjectionMatrix(Camera* self, float width, float height)
{
    self->width = width;
    self->height = height;
    self->proj = glms_ortho(-(self->width/2) * self->zoom, (self->width/2) * self->zoom, -(self->height/2) * self->zoom, (self->height/2) * self->zoom, -self->zoom, self->zoom);
    self->projview = glms_mat4_mul(self->proj, self->view);
}