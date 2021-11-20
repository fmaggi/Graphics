#include "game/game.h"
#include "mainLayer.h"

#include "iostream"

int main()
{
    std::cout << "starting" << std::endl;

    Game::SetUp(new Test);
    Game::Run();
    Game::Destroy();
}
