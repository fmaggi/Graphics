#ifndef SCRIPT_H
#define SCRIPT_H

#include "engine.h"

struct LayerData
{
    TextureID t;
    EntityID player;
    uint32_t width = 0, height = 0;
    bool renderUI = true;
};

template<>
bool Layer::OnEvent<KeyPressed>(KeyPressed event);

template<>
bool Layer::OnEvent<MouseMoved>(MouseMoved event);

template<>
bool Layer::OnEvent<WindowResize>(WindowResize event);

#endif
