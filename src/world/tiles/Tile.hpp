#ifndef CPPDUNGEON_WORLD_TILES_TILE_HPP
#define CPPDUNGEON_WORLD_TILES_TILE_HPP

#include "../../constants.hpp"
#include "olcPixelGameEngine.h"

#include <memory>

namespace cppdungeon
{
    namespace world
    {
        namespace tiles
        {
            class Tile;
        }
    }
}

class cppdungeon::world::tiles::Tile
{
protected:
    std::unique_ptr<olc::Sprite> texture;
    olc::vf2d size;
    bool isSolid = false;
public:
    static constexpr f32 WIDTH = 16;
    static constexpr f32 HEIGHT = 16;

    Tile(std::string texture);
    ~Tile();
    void render(olc::PixelGameEngine* pge, olc::vf2d position);
    void setSolid(bool isSolid);
};

#endif