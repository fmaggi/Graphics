#ifndef GAME_H
#define GAME_H

#include "layer.h"

class Game
{
public:
    static bool SetUp(Layer* layer);
    static void Run();
    static void Destroy();
    static void OnUpdate(float ts);
    static void OnRender();
private:
};

#endif
