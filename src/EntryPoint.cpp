#include "game/game.h"
#include "log/log.h"

extern GameDef& GetGameSpecs();

int main()
{
    LOG_TRACE("Starting Engine");
    Game::SetUp(GetGameSpecs());
    Game::Run();
}
