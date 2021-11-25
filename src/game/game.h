#ifndef GAME_H
#define GAME_H

#include <string>

class Game
{
public:
    static bool SetUp(uint32_t width, uint32_t hieght, const std::string& title);
    static void Run();
    static void Destroy();
    static void OnUpdate(float ts);
    static void OnRender();
};

#endif
