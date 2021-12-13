#include "game/game.h"
#include "runtimeScript.h"

GameDef Game::GetGameDef()
{
    GameDef def {
        .baseLayer = new MyLayer,
        .width = 1200, .height = 800,
        .title = "Engine"
    };
    return def;
}
