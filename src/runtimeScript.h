#ifndef SCRIPT_H
#define SCRIPT_H

#include "engine.h"

struct LayerData
{
    TextureID t;
    glm::vec3 pos{0,0,0};
};

template<>
bool Layer::OnEvent<KeyPressed>(KeyPressed event);

#endif
