#ifndef CPPDUNGEON_HPP
#define CPPDUNGEON_HPP
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "constants.hpp"
#include <chrono>

namespace cppdungeon
{
    class Game;
}

class cppdungeon::Game : public olc::PixelGameEngine
{
public:
    f32 m_dt = 0;
    bool running = true;

    Game()
    {
        sAppName = "dungeon";
    }

    void handleEvents();
    void update();
    void render();
    void cleanup();

private:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // called once per frame
        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++)
                Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand() % 255));
        return true;
    }
};

#endif