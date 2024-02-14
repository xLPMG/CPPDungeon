#ifndef CPPDUNGEON_HPP
#define CPPDUNGEON_HPP

#include "constants.hpp"
#include "entities/Player.hpp"

#include <SDL.h>
#include <chrono>

namespace cppdungeon
{
    class Game;
}

class cppdungeon::Game
{
public:
    f32 m_dt = 0;
    bool running = true;

    void handleEvents();
    void update();
    void render();
    void cleanup();
    int start(int width, int height);

private:    
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture, *debug;
    cppdungeon::entities::Player *m_player;
};

#endif