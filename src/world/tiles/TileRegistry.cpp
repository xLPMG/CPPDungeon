#include "TileRegistry.hpp"
#include "PlaceholderTile.hpp"

cppdungeon::world::tiles::TileRegistry::TileRegistry()
{
    tiles[0] = new PlaceholderTile();
    tiles[0]->setSolid(true);
    tiles[1] = new Tile("./res/textures/map/floor_plain.png");
}

cppdungeon::world::tiles::TileRegistry::~TileRegistry()
{
    for(int i =0; i< cppdungeon::world::tiles::TileRegistry::TILE_COUNT; i++){
        delete tiles[i];
    }
}