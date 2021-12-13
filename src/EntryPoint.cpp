#include "game/game.h"
#include "log/log.h"

extern GameDef Game::GetGameDef();

int main()
{
    LOG_TRACE("Starting Engine");
    Game::SetUp(Game::GetGameDef());
    Game::Run();
}
