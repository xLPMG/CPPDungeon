#ifndef GAME_HPP
#define GAME_HPP

#define OLC_IGNORE_VEC2D
#include "olcUTIL_Geometry2D.h"

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
    std::unique_ptr<cppdungeon::world::tiles::TileRegistry> tileRegistry;
    std::unique_ptr<cppdungeon::world::Map> map;
    std::unique_ptr<cppdungeon::gfx::Camera> camera;
    std::unique_ptr<cppdungeon::entities::Entity> player;

    i32 seed = 1;
    
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
    bool OnUserDestroy() override;

    gfx::Camera *getCamera(){
        return this->camera.get();
    }
};

#endif