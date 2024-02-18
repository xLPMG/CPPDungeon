#ifndef CPPDUNGEON_WORLD_TILES_TILEREGISTRY_HPP
#define CPPDUNGEON_WORLD_TILES_TILEREGISTRY_HPP

#include "../../constants.hpp"
#include "Tile.hpp"
#include <vector>
#include <memory>

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
    std::vector<std::unique_ptr<cppdungeon::world::tiles::Tile>> tiles;
    TileRegistry();
    ~TileRegistry();
};

#endif