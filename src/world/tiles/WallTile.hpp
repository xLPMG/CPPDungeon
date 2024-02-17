#ifndef WALLTILE_HPP
#define WALLTILE_HPP

#include "Tile.hpp"

namespace cppdungeon
{
    namespace world
    {
        namespace tiles
        {
            class WallTile;
        }
    }
}

class cppdungeon::world::tiles::WallTile : public Tile
{
public:
    WallTile(std::string texture) : Tile(texture)
    {
        this->setSolid(true);
    };
    void render(olc::PixelGameEngine *pge, olc::vf2d position) override
    {
        pge->DrawSprite(position, texture.get());
    }
};

#endif