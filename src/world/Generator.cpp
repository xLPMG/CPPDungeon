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
    regions.resize(width * height);
    generateRooms(tiles);

    for (i32 y = bounds.pos.y; y < bounds.size.y; y += 2)
    {
        for (i32 x = bounds.pos.x; x < bounds.size.x; x += 2)
        {
            if (tiles[x + y * width] != 0)
                continue;
            growMaze(olc::vi2d(x, y), tiles);
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
        std::normal_distribution<float> heightDist(w, roomSizeVariance/2);
        i32 h = (i32)heightDist(generator);
        if(w%2 == 0)
            w++;
        if(h%2 == 0)
            h++;

        if (w < 5 || h < 5)
        {
            continue;
        }

        i32 x = rand() % (bounds.size.x - w) + bounds.pos.x;
        i32 y = rand() % (bounds.size.y - h) + bounds.pos.y;
        if(x%2 == 0)
            x++;
        if(y%2 == 0)
            y++;

        olc::utils::geom2d::rect<i32> room;
        room.pos = {x, y};
        room.size = {w, h};

        olc::utils::geom2d::rect<i32> roomWithPadding;
        roomWithPadding.pos = {x - 4, y - 4};
        roomWithPadding.size = {w + 8, h + 8};

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
            startRegion();
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

void cppdungeon::world::Generator::growMaze(olc::vi2d start, std::vector<u16> &tiles)
{
    std::vector<olc::vi2d> cells;
    olc::vi2d lastDir = {0, 0};
    startRegion();
    carve(start, 1, tiles);
    cells.push_back(start);

    while (!cells.empty())
    {
        olc::vi2d cell = cells.back();
        std::vector<olc::vi2d> unmadeCells;
        for (auto &dir : dirs)
        {
            if (canCarve(cell, dir, tiles))
            {
                unmadeCells.push_back(dir);
            }
        }
        if (!unmadeCells.empty())
        {
            // Based on how "windy" passages are, try to prefer carving in the
            // same direction.
            olc::vi2d dir;
            if (std::find(unmadeCells.begin(), unmadeCells.end(), lastDir) != unmadeCells.end() && rand() % 100 > windingPercent)
            {
                dir = lastDir;
            }
            else
            {
                dir = unmadeCells[rand() % unmadeCells.size()];
            }
            
            carve(cell + dir, 1, tiles);
            carve(cell + dir * 2, 1, tiles);
            cells.push_back(cell + dir * 2);
            lastDir = dir;
        }
        else
        {
            // No adjacent uncarved cells.
            cells.pop_back();
            lastDir = {0,0};
        }
    }
}

bool cppdungeon::world::Generator::canCarve(olc::vi2d pos, olc::vi2d direction, std::vector<u16> &tiles)
{
  if(!contains(bounds, pos + direction * 3))
  {
    return false;
  }

  return tiles[(pos + direction * 2).y * width + (pos + direction * 2).x] == 0;
}

void cppdungeon::world::Generator::startRegion()
{
    currentRegion++;
}

void cppdungeon::world::Generator::carve(olc::vi2d pos, u16 tile, std::vector<u16> &tiles)
{
    tiles[pos.y * width + pos.x] = tile;
}