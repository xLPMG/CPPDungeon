#ifndef CPPDUNGEON_WORLD_MAP_HPP
#define CPPDUNGEON_WORLD_MAP_HPP

#include "../constants.hpp"
#include "olcPixelGameEngine.h"
#include <memory>

namespace cppdungeon
{
    namespace world {
        class Map;
    }
}

class cppdungeon::world::Map
{
    private:
        i32 width = 100;
        i32 height = 100;
        olc::vf2d tileSize = {16, 16};
        std::unique_ptr<i32[]> tiles;

    public:
        Map(i32 seed, i32 width, i32 height, olc::vf2d tileSize);
        void update(float fElapsedTime);
        void renderBackground(olc::PixelGameEngine *pge, olc::vf2d offset, olc::vf2d screenSize);
        void renderForeground(olc::PixelGameEngine *pge, olc::vf2d offset, olc::vf2d screenSize);
};

#endif