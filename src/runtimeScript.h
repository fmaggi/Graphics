#ifndef SCRIPT_H
#define SCRIPT_H

#include "engine.h"

struct LayerData
{
    TextureID t;
    EntityID player;
};

template<>
bool Layer::OnEvent<KeyPressed>(KeyPressed event);

template<>
bool Layer::OnEvent<MouseMoved>(MouseMoved event);

#endif
