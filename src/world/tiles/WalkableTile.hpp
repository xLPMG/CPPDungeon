#ifndef WALKABLETILE_HPP
#define WALKABLETILE_HPP

#include "Tile.hpp"

namespace cppdungeon
{
    namespace world
    {
        namespace tiles
        {
            class WalkableTile;
        }
    }
}

class cppdungeon::world::tiles::WalkableTile : public Tile
{
public:
    WalkableTile(std::string texture) : Tile(texture)
    {
        this->setSolid(false);
    }
    ~WalkableTile()
    {
        // nothing to delete
    }
    void render(olc::PixelGameEngine *pge, olc::vf2d position) override
    {
        pge->DrawSprite(position, texture.get());
    }
};

#endif