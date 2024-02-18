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

using recti32 = olc::utils::geom2d::rect<i32>;

private:
    static constexpr olc::vi2d N = {0, -1};
    static constexpr olc::vi2d S = {0, 1};
    static constexpr olc::vi2d W = {-1, 0};
    static constexpr olc::vi2d E = {1, 0};
    std::vector<olc::vi2d> dirs = {N, S, W, E};

    // map data
    i32 width{};
    i32 height{};
    recti32 bounds{{0, 0}, {0, 0}};
    std::vector<usize> *tilesBackgroundPtr;
    std::vector<usize> *tilesForegroundPtr;

    // room generation
    std::vector<recti32> rooms;
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
    i32 loopProbability = 5;

    void generateRooms();
    void carve(olc::vi2d pos,
               u16 tile,
               std::vector<usize> &tiles);
    void connectPoints(olc::vi2d a,
                       olc::vi2d b);
    void constructDelauneyTriangles(std::map<olc::vi2d,
                                             std::vector<olc::vi2d>> &graph);
    void dfsSpanningTree(std::map<olc::vi2d, std::vector<olc::vi2d>> &graph,
                         const olc::vi2d &start,
                         std::vector<std::pair<olc::vi2d, olc::vi2d>> &spanningTree);
    void fillGaps();

    void buildWalls();

    void decorateFloor();

public:
    void generate(u32 seed,
                  u16 width,
                  u16 height,
                  std::vector<usize> &tilesBackground,
                  std::vector<usize> &tilesForeground,
                  olc::vf2d &spawnPos);
};

#endif