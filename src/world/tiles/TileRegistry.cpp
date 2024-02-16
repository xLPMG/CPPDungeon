#include "TileRegistry.hpp"
#include "WalkableTile.hpp"
#include "PlaceholderTile.hpp"

cppdungeon::world::tiles::TileRegistry::TileRegistry()
{
    tiles.resize(2);
    tiles[0] = new PlaceholderTile();
    tiles[1] = new WalkableTile("./res/textures/map/floor_plain.png");
}

cppdungeon::world::tiles::TileRegistry::~TileRegistry()
{
    for (auto t : tiles)
    {
        delete t;
    }
    tiles.clear();
}