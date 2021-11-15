#ifndef ENTITY_ACTIONS_H
#define ENTITY_ACTIONS_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

static inline glm::mat4 getTransform(glm::vec3 translation, float rotation, glm::vec2 scale)
{
    glm::mat4 m = glm::translate(glm::mat4(1.0f), translation);
    m = glm::rotate(m, rotation, {0, 0, 1});
    m = glm::scale(m, {scale.x, scale.y, 1});
    return m;
}

#endif
