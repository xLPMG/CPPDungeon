#ifndef STAIRSTILE_HPP
#define STAIRSTILE_HPP

#include "Tile.hpp"

namespace cppdungeon
{
    namespace world
    {
        namespace tiles
        {
            class StairsTile;
        }
    }
}

class cppdungeon::world::tiles::StairsTile : public Tile
{
private:
    olc::vi2d spriteOffset;
    bool flip;

public:
    StairsTile(std::string texture, olc::vi2d spriteOffset, bool flip) : Tile(texture), spriteOffset(spriteOffset), flip(flip)
    {
        this->setSolid(false);
    }

    ~StairsTile()
    {
        // nothing to delete
    }
    void render(olc::PixelGameEngine *pge, olc::vf2d position) override
    {
        pge->DrawPartialSprite(position, texture.get(), spriteOffset, TILE_SIZE, 1.0, flip);
    }
};

#endif