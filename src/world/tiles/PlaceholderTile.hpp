#ifndef PLACEHOLDERTILE_HPP
#define PLACEHOLDERTILE_HPP

#include "Tile.hpp"

namespace cppdungeon
{
    namespace world
    {
        namespace tiles
        {
            class PlaceholderTile;
        }
    }
}

class cppdungeon::world::tiles::PlaceholderTile : public Tile
{
public:
    PlaceholderTile(): Tile(""){
        this->setSolid(true);
    }
    ~PlaceholderTile(){
        // nothing to delete
    }
    void render(olc::PixelGameEngine *pge, olc::vf2d position) override {
        // dont draw anything
    }
};

#endif