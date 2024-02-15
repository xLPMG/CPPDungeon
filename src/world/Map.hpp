#ifndef CPPDUNGEON_WORLD_MAP_HPP
#define CPPDUNGEON_WORLD_MAP_HPP

#include "../constants.hpp"
#include "olcPixelGameEngine.h"
#include "tiles/Tile.hpp"
#include "tiles/TileRegistry.hpp"
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
    i32 *tiles;
    cppdungeon::world::tiles::TileRegistry *tileRegistry;

public:
    olc::vf2d tileSize = {16, 16};
    i32 width = 100;
    i32 height = 100;
    Map(i32 seed, i32 width, i32 height, olc::vf2d tileSize, cppdungeon::world::tiles::TileRegistry *tileRegistry);
    void update(float fElapsedTime);
    void renderBackground(olc::PixelGameEngine *pge, olc::vf2d offset, olc::vf2d screenSize);
    void renderForeground(olc::PixelGameEngine *pge, olc::vf2d offset, olc::vf2d screenSize);
    bool collides(olc::vf2d coordinate, u32 &tileId);
    rectu getTileBounds(u32 tileid);
};

#endif