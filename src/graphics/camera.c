#include "camera.h"

#include "log/log.h"

Camera orthoCamera(vec3s pos, float width, float height)
{
    Camera self;
    self.speed = (vec2s){100, 100};
    self.pos = pos;
    self.width = width;
    self.height = height;
    self.proj = glms_ortho(0, width, 0, height, -1, 1);
    updateViewMatrix(&self);
    return self;
}

void updateViewMatrix(Camera* self)
{
    mat4s transform = glms_translate(glms_mat4_identity(), self->pos);
    mat4s view = glms_mat4_inv_fast(transform);
    self->view = (view);
    self->projview = glms_mat4_mul(self->proj, self->view);
}

void updateProjectionMatrix(Camera* self, float width, float height)
{
    self->width = width;
    self->height = height;
    self->proj = glms_ortho(0, self->width, 0, self->height, -1, 1);
    self->projview = glms_mat4_mul(self->proj, self->view);
}