#include "TileRegistry.hpp"
#include "WalkableTile.hpp"
#include "PlaceholderTile.hpp"
#include "WallTile.hpp"

cppdungeon::world::tiles::TileRegistry::TileRegistry()
{
    tiles.resize(22);
    tiles[0] = new PlaceholderTile();
    tiles[1] = new WalkableTile("./res/textures/map/floor_plain.png");

    // wall tiles
    tiles[2] = new WallTile("./res/textures/map/wall/Wall_front.png");
    tiles[3] = new WallTile("./res/textures/map/wall/Wall_outer_e.png");
    tiles[4] = new WallTile("./res/textures/map/wall/Wall_outer_e2.png");
    tiles[5] = new WallTile("./res/textures/map/wall/Wall_outer_n.png");
    tiles[6] = new WallTile("./res/textures/map/wall/Wall_outer_ne.png");
    tiles[7] = new WallTile("./res/textures/map/wall/Wall_outer_nw.png");
    tiles[8] = new WallTile("./res/textures/map/wall/Wall_outer_se.png");
    tiles[9] = new WallTile("./res/textures/map/wall/Wall_outer_sw.png");
    tiles[10] = new WallTile("./res/textures/map/wall/Wall_outer_w.png");
    tiles[11] = new WallTile("./res/textures/map/wall/Wall_outer_w2.png");
    tiles[12] = new WallTile("./res/textures/map/wall/Wall_inner_e.png");
    tiles[13] = new WallTile("./res/textures/map/wall/Wall_inner_ne.png");
    tiles[14] = new WallTile("./res/textures/map/wall/Wall_inner_nw.png");
    tiles[15] = new WallTile("./res/textures/map/wall/Wall_inner_se.png");
    tiles[16] = new WallTile("./res/textures/map/wall/Wall_inner_sw.png");
    tiles[17] = new WallTile("./res/textures/map/wall/Wall_inner_w.png");
    tiles[18] = new WallTile("./res/textures/map/wall/Wall_inner_front.png");
    tiles[19] = new WallTile("./res/textures/map/wall/Wall_inner_n.png");
    tiles[20] = new WallTile("./res/textures/map/wall/Wall_front_left.png");
    tiles[21] = new WallTile("./res/textures/map/wall/Wall_front_right.png");
}

cppdungeon::world::tiles::TileRegistry::~TileRegistry()
{
    for (auto t : tiles)
    {
        delete t;
    }
    tiles.clear();
}