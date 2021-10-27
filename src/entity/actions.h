#ifndef ENTITY_ACTIONS_H
#define ENTITY_ACTIONS_H

#include "cglm/struct.h"

static inline mat4s getTransform(vec3s translation, float rotation, vec2s scale)
{
    mat4s m = glms_translate(glms_mat4_identity(), translation);
    m = glms_rotate(m, rotation, (vec3s){{0, 0, 1}});
    vec3s scale3 = (vec3s){{scale.x, scale.y, 1}};
    m = glms_scale(m, scale3);
    return m;
}

#endif
