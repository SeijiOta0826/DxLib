#include "Game.hpp"

#include <memory>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    std::unique_ptr<Game> game = std::make_unique<Game>();
    game->Run();

    return 0;
}