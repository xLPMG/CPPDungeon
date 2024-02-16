#include "Generator.hpp"
#include <random>
#include <algorithm>
#include <iostream>

cppdungeon::world::Generator::Generator(i32 seed) : seed(seed)
{
}

void cppdungeon::world::Generator::generate(i32 width, i32 height, std::vector<u16> &tiles)
{
    srand(seed);
    tiles.clear();
    tiles.resize(width * height);
    this->width = width;
    this->height = height;
    bounds.pos = {1, 1};
    bounds.size = {width - 1, height - 1};

    generateRooms(tiles);

    for (i32 y = bounds.pos.y; y < bounds.size.y; y += 2)
    {
        for (i32 x = bounds.pos.x; x < bounds.size.x; x += 2)
        {
            if (tiles[x + y * width] != 0)
                continue;
        }
    }
}

void cppdungeon::world::Generator::generateRooms(std::vector<u16> &tiles)
{
    rooms.clear();
    std::default_random_engine generator;
    std::normal_distribution<float> widthDist(meanRoomSize, roomSizeVariance);
    for (i32 i = 0; i < numRoomTries; i++)
    {
        // generate room
        i32 w = (i32)widthDist(generator);
        std::normal_distribution<float> heightDist(w, roomSizeVariance / 2);
        i32 h = (i32)heightDist(generator);
        if (w < 6 || h < 6 || w > 25 || h > 25)
        {
            continue;
        }

        if (w % 2 == 0)
            w++;
        if (h % 2 == 0)
            h++;

        i32 x = rand() % (bounds.size.x - w) + bounds.pos.x;
        i32 y = rand() % (bounds.size.y - h) + bounds.pos.y;
        if (x % 2 == 0)
            x++;
        if (y % 2 == 0)
            y++;

        olc::utils::geom2d::rect<i32> room;
        room.pos = {x, y};
        room.size = {w, h};

        olc::utils::geom2d::rect<i32> roomWithPadding;
        roomWithPadding.pos = {x - 2, y - 2};
        roomWithPadding.size = {w + 4, h + 4};

        bool doesOverlap = false;
        for (auto &r : rooms)
        {
            if (overlaps(roomWithPadding, r))
            {
                doesOverlap = true;
                break;
            }
        }
        if (!doesOverlap)
        {
            rooms.push_back(room);
            for (i32 x = room.pos.x; x < room.pos.x + room.size.x; x++)
            {
                for (i32 y = room.pos.y; y < room.pos.y + room.size.y; y++)
                {
                    carve(olc::vi2d(x, y), 1, tiles);
                }
            }
        }
    }
}

void cppdungeon::world::Generator::carve(olc::vi2d pos, u16 tile, std::vector<u16> &tiles)
{
    tiles[pos.y * width + pos.x] = tile;
}