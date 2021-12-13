#ifndef GAME_H
#define GAME_H

#include <string>

class Layer;

struct GameDef
{
    Layer* baseLayer;
    uint32_t width, height;
    std::string title;
};

namespace Game {

    bool SetUp(GameDef def);
    void Run();
    void Destroy();
    void OnUpdate(float ts);
    void OnRender();

    GameDef GetGameDef();
};


#endif
