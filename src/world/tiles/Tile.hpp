#ifndef CPPDUNGEON_WORLD_TILES_TILE_HPP
#define CPPDUNGEON_WORLD_TILES_TILE_HPP

#include "../../constants.hpp"
#define OLC_IGNORE_VEC2D
#include "olcUTIL_Geometry2D.h"
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
    Tile(std::string texture);
    
    virtual ~Tile();
    virtual void render(olc::PixelGameEngine *pge, olc::vf2d position) = 0;
    bool getSolid()
    {
        return this->isSolid;
    };
    void setSolid(bool isSolid)
    {
        this->isSolid = isSolid;
    }
};

#endif