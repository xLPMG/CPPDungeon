#include "TileRegistry.hpp"
#include "WalkableTile.hpp"
#include "PlaceholderTile.hpp"

cppdungeon::world::tiles::TileRegistry::TileRegistry()
{
    tiles[0] = new PlaceholderTile();
    tiles[1] = new WalkableTile("./res/textures/map/floor_plain.png");
}

cppdungeon::world::tiles::TileRegistry::~TileRegistry()
{
    for(int i =0; i< cppdungeon::world::tiles::TileRegistry::TILE_COUNT; i++){
        delete tiles[i];
    }
}