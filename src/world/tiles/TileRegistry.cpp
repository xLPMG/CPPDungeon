#include "TileRegistry.hpp"
#include "WalkableTile.hpp"
#include "PlaceholderTile.hpp"
#include "WallTile.hpp"
#include "PillarTile.hpp"
#include "StairsTile.hpp"

cppdungeon::world::tiles::TileRegistry::TileRegistry()
{
    tiles.resize(53);
    tiles[0] = std::make_unique<PlaceholderTile>();
    tiles[1] = std::make_unique<WalkableTile>("./res/textures/map/floor/floor_plain.png");

    // wall tiles
    tiles[2] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_front.png");
    tiles[3] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_outer_e.png");
    tiles[4] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_outer_e2.png");
    tiles[5] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_outer_n.png");
    tiles[6] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_outer_ne.png");
    tiles[7] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_outer_nw.png");
    tiles[8] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_outer_se.png");
    tiles[9] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_outer_sw.png");
    tiles[10] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_outer_w.png");
    tiles[11] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_outer_w2.png");
    tiles[12] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_inner_e.png");
    tiles[13] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_inner_ne.png");
    tiles[14] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_inner_nw.png");
    tiles[15] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_inner_se.png");
    tiles[16] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_inner_sw.png");
    tiles[17] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_inner_w.png");
    tiles[18] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_inner_front.png");
    tiles[19] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_inner_n.png");
    tiles[20] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_front_left.png");
    tiles[21] = std::make_unique<WallTile>("./res/textures/map/wall/Wall_front_right.png");

    tiles[22] = std::make_unique<WalkableTile>("./res/textures/map/floor/floor_mud_mid_1.png");
    tiles[23] = std::make_unique<WalkableTile>("./res/textures/map/floor/floor_mud_mid_2.png");
    tiles[24] = std::make_unique<WalkableTile>("./res/textures/map/floor/floor_mud_n_1.png");
    tiles[25] = std::make_unique<WalkableTile>("./res/textures/map/floor/floor_mud_n_2.png");
    tiles[26] = std::make_unique<WalkableTile>("./res/textures/map/floor/floor_mud_ne.png");
    tiles[27] = std::make_unique<WalkableTile>("./res/textures/map/floor/floor_mud_nw.png");
    tiles[28] = std::make_unique<WalkableTile>("./res/textures/map/floor/floor_mud_s_1.png");
    tiles[29] = std::make_unique<WalkableTile>("./res/textures/map/floor/floor_mud_s_2.png");
    tiles[30] = std::make_unique<WalkableTile>("./res/textures/map/floor/floor_mud_se.png");
    tiles[31] = std::make_unique<WalkableTile>("./res/textures/map/floor/floor_mud_sw.png");
    tiles[32] = std::make_unique<WalkableTile>("./res/textures/map/floor/floor_mud_w.png");
    tiles[33] = std::make_unique<WalkableTile>("./res/textures/map/floor/floor_mud_e.png");
    tiles[34] = std::make_unique<WalkableTile>("./res/textures/map/floor/floor_ladder.png");

    tiles[35] = std::make_unique<PillarTile>("./res/textures/map/deco/pillar.png", false, olc::vi2d(0, 0));
    tiles[36] = std::make_unique<PillarTile>("./res/textures/map/deco/pillar.png", false, olc::vi2d(0, 16));
    tiles[37] = std::make_unique<PillarTile>("./res/textures/map/deco/pillar.png", false, olc::vi2d(0, 32));
    tiles[38] = std::make_unique<PillarTile>("./res/textures/map/deco/pillarLadder.png", false, olc::vi2d(0, 0));
    tiles[39] = std::make_unique<PillarTile>("./res/textures/map/deco/pillarLadder.png", false, olc::vi2d(0, 16));
    tiles[40] = std::make_unique<PillarTile>("./res/textures/map/deco/pillarLadder.png", false, olc::vi2d(0, 32));
    tiles[41] = std::make_unique<PillarTile>("./res/textures/map/deco/pillarLadderBg.png", true, olc::vi2d(0, 0));
    tiles[42] = std::make_unique<PillarTile>("./res/textures/map/deco/pillarLadderBg.png", true, olc::vi2d(0, 16));
    tiles[43] = std::make_unique<PillarTile>("./res/textures/map/deco/pillarLadderBg.png", true, olc::vi2d(0, 32));

    // VERTICAL STAIRS
    tiles[44] = std::make_unique<StairsTile>("./res/textures/map/floor/stairs_top.png", olc::vi2d(0, 0), false);  // top left
    tiles[45] = std::make_unique<StairsTile>("./res/textures/map/floor/stairs_top.png", olc::vi2d(8, 0), false);  // top center
    tiles[46] = std::make_unique<StairsTile>("./res/textures/map/floor/stairs_top.png", olc::vi2d(16, 0), false); // top right

    tiles[47] = std::make_unique<StairsTile>("./res/textures/map/floor/stairs_mid.png", olc::vi2d(0, 0), false);  // middle left
    tiles[48] = std::make_unique<StairsTile>("./res/textures/map/floor/stairs_mid.png", olc::vi2d(8, 0), false);  // middle center
    tiles[49] = std::make_unique<StairsTile>("./res/textures/map/floor/stairs_mid.png", olc::vi2d(16, 0), false); // middle right

    tiles[50] = std::make_unique<StairsTile>("./res/textures/map/floor/stairs_bottom.png", olc::vi2d(0, 0), false);  // bottom left
    tiles[51] = std::make_unique<StairsTile>("./res/textures/map/floor/stairs_bottom.png", olc::vi2d(8, 0), false);  // bottom center
    tiles[52] = std::make_unique<StairsTile>("./res/textures/map/floor/stairs_bottom.png", olc::vi2d(16, 0), false); // bottom right
}

cppdungeon::world::tiles::TileRegistry::~TileRegistry()
{
    tiles.clear();
}
