#ifndef PILLAARTILE_HPP
#define PILLAARTILE_HPP

#include "Tile.hpp"

namespace cppdungeon
{
    namespace world
    {
        namespace tiles
        {
            class PillarTile;
        }
    }
}

class cppdungeon::world::tiles::PillarTile : public Tile
{
private:
    olc::vi2d spriteOffset;

public:
    PillarTile(std::string texture, bool isSolid, olc::vi2d spriteOffset) : Tile(texture), spriteOffset(spriteOffset)
    {
        this->setSolid(isSolid);
    }

    ~PillarTile()
    {
        // nothing to delete
    }
    void render(olc::PixelGameEngine *pge, olc::vf2d position) override
    {
        pge->DrawPartialSprite(position, texture.get(), spriteOffset, TILE_SIZE);
    }
};

#endif