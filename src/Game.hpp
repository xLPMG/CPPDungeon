#ifndef GAME_HPP
#define GAME_HPP

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "constants.hpp"
#include "gfx/Camera.hpp"
#include "entities/Entity.hpp"
#include "world/Map.hpp"
#include "world/tiles/TileRegistry.hpp"
#include <chrono>
#include <memory>

namespace cppdungeon
{
    class Game;
}

class cppdungeon::Game : public olc::PixelGameEngine
{
public:
    Game()
    {
        sAppName = "dungeon";
    }
private:    
    gfx::Camera* camera;
    olc::vi2d vBlockSize = { 16,16 };
    std::vector<entities::Entity *> entities;
    cppdungeon::world::tiles::TileRegistry* tileRegistry;
    world::Map* map;

    entities::Entity *player;
    
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
    bool OnUserDestroy() override;

    gfx::Camera *getCamera(){
        return this->camera;
    }
};

#endif