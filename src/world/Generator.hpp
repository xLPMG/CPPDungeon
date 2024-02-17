#ifndef CPPDUNGEON_WORLD_GENERATOR_HPP
#define CPPDUNGEON_WORLD_GENERATOR_HPP

#include "../constants.hpp"
#define OLC_IGNORE_VEC2D
#include "olcUTIL_Geometry2D.h"
#include <vector>
#include <map>

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
    f32 meanRoomSize = 17;
    f32 roomSizeVariance = 10;

    // path generation
    class VI2dHash
    {
    private:
        const int HASH_SIZE = 100;
        const float L = 0.2f;

    public:
        std::size_t operator()(olc::vi2d const &v) const
        {
            int ix = (unsigned int)((v.x + 2.f) / L);
            int iy = (unsigned int)((v.y + 2.f) / L);
            return (unsigned int)((ix * 73856093) ^ (iy * 19349663)) % HASH_SIZE;
        }
    };
    i32 loopProbability = 10;

    void generateRooms(std::vector<u16> &tiles);
    void carve(olc::vi2d pos,
               u16 tile,
               std::vector<u16> &tiles);
    void connectPoints(olc::vi2d a,
                       olc::vi2d b,
                       std::vector<u16> &tiles);
    void carveRect(olc::utils::geom2d::rect<i32> rect,
                   u16 tile,
                   std::vector<u16> &tiles);
    void constructDelauneyTriangles(std::map<olc::vi2d,
                                             std::vector<olc::vi2d>> &graph);
    void bfsSpanningTree(std::map<olc::vi2d, std::vector<olc::vi2d>> &graph,
                         const olc::vi2d &start,
                         std::vector<std::pair<olc::vi2d, olc::vi2d>> &spanningTree);

public:
    Generator(i32 seed);
    void generate(i32 width,
                  i32 height,
                  std::vector<u16> &tiles);
};

#endif