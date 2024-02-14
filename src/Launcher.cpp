#include "Game.hpp"
#include "constants.hpp"

#include <SDL.h>
#include <iostream>

int main(int argc, char *argv[])
{
    cppdungeon::Game *game = new cppdungeon::Game();
    std::cout << "Starting game" << std::endl;
    game->start(1280, 720);

    cppdungeon::u32 start;
    while (game->running)
    {
        start = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        game->m_dt = (SDL_GetTicks() - start) / 100.0f;
    }
    game->cleanup();
}
