#include "game/game.h"
#include "mainLayer.h"

int main()
{
    Game::SetUp(new Test);
    Game::Run();
}
