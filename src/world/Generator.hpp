#ifndef CPPDUNGEON_WORLD_GENERATOR_HPP
#define CPPDUNGEON_WORLD_GENERATOR_HPP

#include "../constants.hpp"
#define OLC_IGNORE_VEC2D
#include "olcUTIL_Geometry2D.h"
#include <vector>

namespace cppdungeon
{
    namespace world
    {
        class Generator;
    }
}

class cppdungeon::world::Generator
{
private:
    const olc::vi2d UP = {0, -1};
    const olc::vi2d DOWN = {0, 1};
    const olc::vi2d LEFT = {-1, 0};
    const olc::vi2d RIGHT = {1, 0};
    const std::vector<olc::vi2d> dirs = {UP, DOWN, LEFT, RIGHT};

    // map data
    i32 seed;
    i32 width;
    i32 height;
    olc::utils::geom2d::rect<i32> bounds;

    // room generation
    std::vector<olc::utils::geom2d::rect<i32>> rooms;
    i32 numRoomTries = 1000;
    f32 meanRoomSize = 15;
    f32 roomSizeVariance = 10;

    void generateRooms(std::vector<u16> &tiles);
    void carve(olc::vi2d pos, u16 tile, std::vector<u16> &tiles);

public:
    Generator(i32 seed);
    void generate(i32 width, i32 height, std::vector<u16> &tiles);
};

#endif