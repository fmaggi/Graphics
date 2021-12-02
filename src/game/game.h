#ifndef GAME_H
#define GAME_H

#include <string>

struct GameDef
{
    uint32_t width, height;
    const std::string& title;
};

// to be defined by the user
GameDef& GetGameSpecs();

class Game
{
public:
    static bool SetUp(GameDef& def);
    static void Run();
    static void Destroy();
    static void OnUpdate(float ts);
    static void OnRender();
};

#endif
