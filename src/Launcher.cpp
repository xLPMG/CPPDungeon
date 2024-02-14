#include "Game.hpp"
#include "constants.hpp"

#include <SDL.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    cppdungeon::Game *game = new cppdungeon::Game();
    std::cout << "Starting game" << std::endl;
    game->start(1280, 720);

    cppdungeon::u32 startTicks;
    while (game->running)
    {
        startTicks = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        game->m_dt = (SDL_GetTicks() - startTicks) / 1000.0f;
    }
    game->cleanup();
}
