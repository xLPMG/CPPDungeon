#include "Generator.hpp"
#define OLC_IGNORE_VEC2D
#include "olcUTIL_Geometry2D.h"
#include <random>
#include <algorithm>
#include <iostream>

void cppdungeon::world::Generator::generate(i32 seed, i32 width, i32 height, std::vector<u16> &tiles)
{
    srand(seed);
    tiles.clear();
    tiles.resize(width * height);

    i32 numRooms = 10;
    std::vector<olc::utils::geom2d::rect<i32>> roomList;
    i32 meanRoomSize = 20;
    i32 roomSizeVariance = 5;
    std::default_random_engine generator;
    std::normal_distribution<double> widthDist(meanRoomSize, roomSizeVariance);

    for (i32 i = 0; i < numRooms; i++)
    {
        olc::utils::geom2d::rect<i32> room;

        i32 w = (i32)widthDist(generator);
        std::normal_distribution<double> heightDist(w, roomSizeVariance);
        i32 h = (i32)heightDist(generator);
        i32 x = rand() % (width - w);
        i32 y = rand() % (height - h);

        room.pos = {x, y};
        room.size = {w, h};
        roomList.push_back((room));
    }

    std::sort(roomList.begin(), roomList.end(), [](const olc::utils::geom2d::rect<i32> &lhs, const olc::utils::geom2d::rect<i32> &rhs)
              { return lhs.area() > rhs.area(); });

    std::vector<olc::utils::geom2d::rect<i32>>::iterator iter;
    for (iter = roomList.begin(); iter != roomList.end();)
    {
        for (i32 y = iter->pos.y; y < iter->pos.y + iter->size.y; y++)
        {
            for (i32 x = iter->pos.x; x < iter->pos.x + iter->size.x; x++)
            {
                tiles[y * width + x] = 1;
            }
        }
        ++iter;
    }
}