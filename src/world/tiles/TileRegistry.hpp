#ifndef CPPDUNGEON_WORLD_TILES_TILEREGISTRY_HPP
#define CPPDUNGEON_WORLD_TILES_TILEREGISTRY_HPP

#include "../../constants.hpp"
#include "Tile.hpp"

namespace cppdungeon
{
    namespace world
    {
        namespace tiles
        {
            class TileRegistry;
        }
    }
}

class cppdungeon::world::tiles::TileRegistry
{
public:
    static const u16 TILE_COUNT = 2;
    cppdungeon::world::tiles::Tile *tiles[TILE_COUNT];
    TileRegistry();
    ~TileRegistry();
};

#endif