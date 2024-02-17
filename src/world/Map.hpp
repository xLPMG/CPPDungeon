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
    std::vector<u16> tiles;
    cppdungeon::world::tiles::TileRegistry *tileRegistry;
    cppdungeon::world::Generator *generator;

public:
    olc::vf2d tileSize = {16, 16};
    i32 width = 99;
    i32 height = 99;
    Map(i32 seed, i32 width, i32 height, olc::vf2d tileSize, cppdungeon::world::tiles::TileRegistry *tileRegistry);
    ~Map();
    void update(float fElapsedTime);
    void renderBackground(olc::PixelGameEngine *pge, olc::vf2d offset, olc::vf2d screenSize);
    void renderForeground(olc::PixelGameEngine *pge, olc::vf2d offset, olc::vf2d screenSize);
    bool collides(olc::vf2d coordinate, u32 &tileId);
    void regenerate(i32 seed);
};

#endif