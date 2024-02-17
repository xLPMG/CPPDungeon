#include "Generator.hpp"
#include <random>
#include <algorithm>
#include <iostream>

#include <delaunator.hpp>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>
#include <cassert>

void cppdungeon::world::Generator::generate(i32 seed, i32 width, i32 height, std::vector<u16> &tiles)
{
    srand(seed);
    tiles.clear();
    tiles.resize(width * height);
    this->width = width;
    this->height = height;
    bounds.pos = {1, 1};
    bounds.size = {width - 2, height - 2};

    generateRooms(tiles);

    for (i32 y = bounds.pos.y; y < bounds.size.y; y += 2)
    {
        for (i32 x = bounds.pos.x; x < bounds.size.x; x += 2)
        {
            if (tiles[x + y * width] != 0)
                continue;
        }
    }
    // generate paths
    std::map<olc::vi2d, std::vector<olc::vi2d>> graph;
    constructDelauneyTriangles(graph);

    std::vector<std::pair<olc::vi2d, olc::vi2d>> spanningTree;
    dfsSpanningTree(graph, graph.begin()->first, spanningTree);

    for (auto &edge : spanningTree)
    {
        olc::vi2d start = edge.first;
        olc::vi2d end = edge.second;
        connectPoints(start, end, tiles);
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

void cppdungeon::world::Generator::connectPoints(olc::vi2d p1, olc::vi2d p2, std::vector<u16> &tiles)
{
    if (p1.x == p2.x || p1.y == p2.y)
    {
        // If aligned horizontally
        if (p1.x == p2.x)
        {
            // Connect horizontally first
            int dy = abs(p2.y - p1.y);
            int signY = (p2.y - p1.y > 0) ? 1 : -1;

            for (int i = 0; i <= dy; ++i){
                carve({p1.x - 1, p1.y + i * signY}, 1, tiles);
                carve({p1.x, p1.y + i * signY}, 1, tiles);
                carve({p1.x + 1, p1.y + i * signY}, 1, tiles);
            }
        }
        // If aligned vertically
        else
        {
            // Connect vertically first
            int dx = abs(p2.x - p1.x);
            int signX = (p2.x - p1.x > 0) ? 1 : -1;

            for (int i = 0; i <= dx; ++i)
            {
                carve({p1.x + i * signX, p1.y - 1}, 1, tiles);
                carve({p1.x + i * signX, p1.y}, 1, tiles);
                carve({p1.x + i * signX, p1.y + 1}, 1, tiles);
            }
        }
    }
    // If not aligned horizontally or vertically
    else
    {
        if (rand() % 2 == 0)
        {
            // Connect horizontally first towards p2
            int dx = abs(p2.x - p1.x);
            int signX = (p2.x - p1.x > 0) ? 1 : -1;

            for (int i = 0; i <= dx; ++i)
            {
                carve({p1.x + i * signX, p1.y - 1}, 1, tiles);
                carve({p1.x + i * signX, p1.y}, 1, tiles);
                carve({p1.x + i * signX, p1.y + 1}, 1, tiles);
            }

            // Then connect vertically towards p2
            int dy = abs(p2.y - p1.y);
            int signY = (p2.y - p1.y > 0) ? 1 : -1;

            for (int i = 0; i <= dy; ++i)
            {
                carve({p2.x - 1, p1.y + i * signY}, 1, tiles);
                carve({p2.x, p1.y + i * signY}, 1, tiles);
                carve({p2.x + 1, p1.y + i * signY}, 1, tiles);
            }
        }
        else
        {
            // Connect vertically first towards p2
            int dy = abs(p2.y - p1.y);
            int signY = (p2.y - p1.y > 0) ? 1 : -1;

            for (int i = 0; i <= dy; ++i)
            {
                carve({p1.x - 1, p1.y + i * signY}, 1, tiles);
                carve({p1.x, p1.y + i * signY}, 1, tiles);
                carve({p1.x + 1, p1.y + i * signY}, 1, tiles);
            }
            // Then connect horizontally towards p2
            int dx = abs(p2.x - p1.x);
            int signX = (p2.x - p1.x > 0) ? 1 : -1;

            for (int i = 0; i <= dx; ++i)
            {
                carve({p1.x + i * signX, p2.y - 1}, 1, tiles);
                carve({p1.x + i * signX, p2.y}, 1, tiles);
                carve({p1.x + i * signX, p2.y + 1}, 1, tiles);
            }
        }
    }
}

void cppdungeon::world::Generator::carve(olc::vi2d pos, u16 tile, std::vector<u16> &tiles)
{
    tiles[pos.y * width + pos.x] = tile;
}

void cppdungeon::world::Generator::carveRect(olc::utils::geom2d::rect<i32> rect, u16 tile, std::vector<u16> &tiles)
{
    for (i32 x = rect.pos.x; x < rect.pos.x + rect.size.x; x++)
    {
        for (i32 y = rect.pos.y; y < rect.pos.y + rect.size.y; y++)
        {
            carve(olc::vi2d(x, y), tile, tiles);
        }
    }
}

void cppdungeon::world::Generator::constructDelauneyTriangles(std::map<olc::vi2d, std::vector<olc::vi2d>> &graph)
{
    graph.clear();
    std::vector<double> coords;
    for (auto &r : rooms)
    {
        coords.push_back(r.middle().x);
        coords.push_back(r.middle().y);
    }
    delaunator::Delaunator d(coords);
    for (i32 i = 0; i < d.triangles.size(); i += 3)
    {
        i32 i0 = d.triangles[i];
        i32 i1 = d.triangles[i + 1];
        i32 i2 = d.triangles[i + 2];
        olc::vi2d p0 = {(i32)coords[i0 * 2], (i32)coords[i0 * 2 + 1]};
        olc::vi2d p1 = {(i32)coords[i1 * 2], (i32)coords[i1 * 2 + 1]};
        olc::vi2d p2 = {(i32)coords[i2 * 2], (i32)coords[i2 * 2 + 1]};
        graph[p0].push_back(p1);
        graph[p0].push_back(p2);
        graph[p1].push_back(p0);
        graph[p1].push_back(p2);
        graph[p2].push_back(p0);
        graph[p2].push_back(p1);
    }
}

void cppdungeon::world::Generator::dfsSpanningTree(std::map<olc::vi2d, std::vector<olc::vi2d>> &graph, const olc::vi2d &start, std::vector<std::pair<olc::vi2d, olc::vi2d>> &spanningTree)
{
    spanningTree.clear();
    std::stack<olc::vi2d> s;
    std::unordered_set<olc::vi2d, VI2dHash> visited;

    s.push(start);
    visited.insert(start);

    while (!s.empty())
    {
        olc::vi2d current = s.top();
        s.pop();
        for (const olc::vi2d &neighbor : graph.at(current))
        {
            if (visited.find(neighbor) == visited.end())
            {
                // add node that we haven't visited yet
                spanningTree.push_back({current, neighbor});
                s.push(neighbor);
                visited.insert(neighbor);
            }
            else if (rand() % 100 < loopProbability)
            {
                // add node that we have visited before
                spanningTree.push_back({current, neighbor});
            }
        }
    }
}