#include "game/game.h"

GameDef& GetGameSpecs()
{
    static GameDef def = {1200, 800, "Title"};
    return def;
}
