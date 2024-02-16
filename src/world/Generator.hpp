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
    i32 numRoomTries = 500;
    f32 meanRoomSize = 15;
    f32 roomSizeVariance = 5;
    /// For each open position in the dungeon, the index of the connected region
    /// that that position is a part of.
    std::vector<i32> regions;
    /// The index of the current region being carved.
    i32 currentRegion = -1;

    /// The inverse chance of adding a connector between two regions that have
    /// already been joined. Increasing this leads to more loosely connected
    /// dungeons.
    i32 extraConnectorChance = 20;
    i32 windingPercent = 0;

    void generateRooms(std::vector<u16> &tiles);
    void growMaze(olc::vi2d start, std::vector<u16> &tiles);
    void startRegion();
    bool canCarve(olc::vi2d pos, olc::vi2d direction, std::vector<u16> &tiles);
    void carve(olc::vi2d pos, u16 tile, std::vector<u16> &tiles);

public:
    Generator(i32 seed);
    void generate(i32 width, i32 height, std::vector<u16> &tiles);
};

#endif