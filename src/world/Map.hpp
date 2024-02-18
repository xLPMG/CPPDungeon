#ifndef CPPDUNGEON_WORLD_MAP_HPP
#define CPPDUNGEON_WORLD_MAP_HPP

#include "../constants.hpp"
#define OLC_IGNORE_VEC2D
#include "olcUTIL_Geometry2D.h"
#include "olcPixelGameEngine.h"
#include "tiles/Tile.hpp"
#include "tiles/TileRegistry.hpp"
#include "Generator.hpp"
#include <vector>
#include <memory>

namespace cppdungeon
{
    namespace world
    {
        class Map;
    }
}

class cppdungeon::world::Map
{
    using Tile = cppdungeon::world::tiles::Tile;

private:
    std::vector<usize> tilesForeground;
    std::vector<usize> tilesBackground;
    cppdungeon::world::tiles::TileRegistry *tileRegistry;
    std::unique_ptr<cppdungeon::world::Generator> generator;
    olc::vf2d spawnPoint = {0, 0};
    u16 width{};
    u16 height{};

public:
    Map(u32 seed, u16 width, u16 height, cppdungeon::world::tiles::TileRegistry *tileRegistry);
    ~Map();
    void update(float fElapsedTime);
    void renderBackground(olc::PixelGameEngine *pge, olc::vf2d offset, olc::vf2d screenSize);
    void renderForeground(olc::PixelGameEngine *pge, olc::vf2d offset, olc::vf2d screenSize);
    bool collides(olc::vf2d coordinate, usize &tileId);
    void regenerate(u32 seed);
    olc::vf2d getSpawnPoint();
    u16 getWidth(){
        return width;
    }
    u16 getHeight(){
        return height;
    }
};

#endif