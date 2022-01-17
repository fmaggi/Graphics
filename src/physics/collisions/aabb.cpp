#include "aabb.h"

#include "log/log.h"

void AABB::Update(glm::vec2 translation)
{
    float hWidth = radius.x;
    float hHeihgt = radius.y;

    min = {translation.x - hWidth, translation.y - hHeihgt};
    max = {translation.x + hWidth, translation.y + hHeihgt};
}

