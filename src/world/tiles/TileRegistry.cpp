#include "TileRegistry.hpp"

cppdungeon::world::tiles::TileRegistry::TileRegistry()
{
    tiles[0] = nullptr;
    tiles[1] = new Tile("./res/textures/map/floor_plain.png");
}

cppdungeon::world::tiles::TileRegistry::~TileRegistry()
{
    for(int i =0; i< cppdungeon::world::tiles::TileRegistry::TILE_COUNT; i++){
        delete tiles[i];
    }
}