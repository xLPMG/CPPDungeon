#include "Generator.hpp"
#include <random>
#include <algorithm>
#include <iostream>

#include <delaunator.hpp>
#include <stack>
#include <unordered_set>
#include <iostream>

void cppdungeon::world::Generator::generate(i32 seed, i32 width, i32 height, std::vector<u16> &tilesBackground, std::vector<u16> &tilesForeground, olc::vf2d &spawnPos)
{
    srand(seed);

    tilesBackground.clear();
    tilesBackground.resize(width * height);
    std::fill(tilesBackground.begin(), tilesBackground.end(), 0);

    tilesForeground.clear();
    tilesForeground.resize(width * height);
    std::fill(tilesForeground.begin(), tilesForeground.end(), 0);

    this->width = width;
    this->height = height;
    bounds.pos = {2, 2};
    bounds.size = {width - 4, height - 4};

    generateRooms(tilesBackground);
    olc::utils::geom2d::rect<i32> spawnRoom = rooms[rand() % rooms.size()];
    spawnPos = spawnRoom.middle();

    // generate paths
    std::map<olc::vi2d, std::vector<olc::vi2d>> graph;
    constructDelauneyTriangles(graph);

    std::vector<std::pair<olc::vi2d, olc::vi2d>> spanningTree;
    dfsSpanningTree(graph, graph.begin()->first, spanningTree);

    for (auto &edge : spanningTree)
    {
        olc::vi2d start = edge.first;
        olc::vi2d end = edge.second;
        connectPoints(start, end, tilesBackground);
    }

    // fill holes
    for (i32 x = bounds.pos.x; x < bounds.pos.x + bounds.size.x; x++)
    {
        for (i32 y = bounds.pos.y; y < bounds.pos.y + bounds.size.y; y++)
        {
            i32 self = idx(x, y, width);
            if (tilesBackground[self] == 0)
            {
                // one tile in each direction
                if (tilesBackground[idx(x + 1, y, width)] > 0 && tilesBackground[idx(x - 1, y, width)] > 0)
                {
                    tilesBackground[self] = 1;
                }
                else if (tilesBackground[idx(x, y + 1, width)] > 0 && tilesBackground[idx(x, y - 1, width)] > 0)
                {
                    tilesBackground[self] = 1;

                    // one and two tiles
                }
                else if (tilesBackground[idx(x, y + 2, width)] > 0 && tilesBackground[idx(x, y - 1, width)] > 0)
                {
                    tilesBackground[self] = 1;
                }
                else if (tilesBackground[idx(x, y + 1, width)] > 0 && tilesBackground[idx(x, y - 2, width)] > 0)
                {
                    tilesBackground[self] = 1;
                }
                else if (tilesBackground[idx(x + 2, y, width)] > 0 && tilesBackground[idx(x - 1, y, width)] > 0)
                {
                    tilesBackground[self] = 1;
                }
                else if (tilesBackground[idx(x + 1, y, width)] > 0 && tilesBackground[idx(x - 2, y, width)] > 0)
                {
                    tilesBackground[self] = 1;
                }
            }
        }
    }

    buildWalls(tilesBackground, tilesForeground);
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

            for (int i = 0; i <= dy; ++i)
            {
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

            // dx+1 to get the corner
            for (int i = 0; i <= dx + 1; ++i)
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

            // dx+1 to get the corner
            for (int i = 0; i <= dy + 1; ++i)
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

void cppdungeon::world::Generator::buildWalls(std::vector<u16> &tilesBackground, std::vector<u16> &tilesForeground)
{

    std::vector<u16> oldBackground = tilesBackground;

    for (i32 x = bounds.pos.x; x < bounds.pos.x + bounds.size.x; x++)
    {
        for (i32 y = bounds.pos.y; y < bounds.pos.y + bounds.size.y; y++)
        {
            olc::vi2d pos = {x, y};
            i32 self = idx(pos, width);
            if (oldBackground[self] == 0)
                continue;

            i32 neighborN = idx(pos + N, width);
            i32 neighborS = idx(pos + S, width);
            i32 neighborW = idx(pos + W, width);
            i32 neighborE = idx(pos + E, width);
            i32 neighborNW = idx(pos + N + W, width);
            i32 neighborNE = idx(pos + N + E, width);
            i32 neighborSW = idx(pos + S + W, width);
            i32 neighborSE = idx(pos + S + E, width);

            // top middle wall
            if (oldBackground[neighborN] == 0 && oldBackground[neighborNW] == 0 && oldBackground[neighborNE] == 0 && oldBackground[neighborW] > 0 && oldBackground[neighborE] > 0)
            {

                tilesBackground[neighborN] = 2;
                tilesBackground[idx(pos + N + N, width)] = 5;
            }
            // bottom middle wall
            if (oldBackground[neighborS] == 0 && oldBackground[neighborSW] == 0 && oldBackground[neighborSE] == 0 && oldBackground[neighborW] > 0 && oldBackground[neighborE] > 0)
            {

                tilesBackground[neighborS] = 18;
                tilesForeground[self] = 19;
            }
            // left center wall
            else if (oldBackground[neighborW] == 0 && oldBackground[neighborNW] == 0 && oldBackground[neighborSW] == 0 && oldBackground[neighborN] > 0 && oldBackground[neighborS] > 0)
            {
                tilesBackground[neighborW] = 10;
            }
            // right center wall
            else if (oldBackground[neighborE] == 0 && oldBackground[neighborNE] == 0 && oldBackground[neighborSE] == 0 && oldBackground[neighborN] > 0 && oldBackground[neighborS] > 0)
            {
                tilesBackground[neighborE] = 3;
            }
            // top left outer corner
            else if (oldBackground[neighborN] == 0 && oldBackground[neighborW] == 0 && oldBackground[neighborNW] == 0)
            {
                tilesBackground[neighborN] = 2;
                tilesBackground[idx(pos + N + N, width)] = 5;

                tilesBackground[neighborW] = 10;
                tilesBackground[neighborNW] = 10;
                tilesBackground[idx(pos + N + N + W, width)] = 7;
            }
            // top right outer corner
            else if (oldBackground[neighborN] == 0 && oldBackground[neighborE] == 0 && oldBackground[neighborNE] == 0)
            {
                tilesBackground[neighborN] = 2;
                tilesBackground[idx(pos + N + N, width)] = 5;

                tilesBackground[neighborE] = 3;
                tilesBackground[neighborNE] = 3;
                tilesBackground[idx(pos + N + N + E, width)] = 6;
            }
            // bottom left outer corner
            else if (oldBackground[neighborS] == 0 && oldBackground[neighborW] == 0 && oldBackground[neighborSW] == 0)
            {
                tilesBackground[neighborS] = 18;
                tilesForeground[self] = 19;

                tilesBackground[neighborSW] = 9;
                tilesBackground[neighborW] = 10;
            }
            // bottom right outer corner
            else if (oldBackground[neighborS] == 0 && oldBackground[neighborE] == 0 && oldBackground[neighborSE] == 0)
            {
                tilesBackground[neighborS] = 18;
                tilesForeground[self] = 19;

                tilesBackground[neighborSE] = 8;
                tilesBackground[neighborE] = 3;

            }
            // top left inner corner
            else if (oldBackground[neighborN] > 0 && oldBackground[neighborW] > 0 && oldBackground[neighborNW] == 0)
            {
                tilesBackground[neighborNW] = 21;
                tilesBackground[idx(pos + N + N + W, width)] = 15;
            }
            // top right inner corner
            else if (oldBackground[neighborN] > 0 && oldBackground[neighborE] > 0 && oldBackground[neighborNE] == 0)
            {
                tilesBackground[neighborNE] = 20;
                tilesBackground[idx(pos + N + N + E, width)] = 16;
            }
            // bottom left inner corner
            else if (oldBackground[neighborS] > 0 && oldBackground[neighborW] > 0 && oldBackground[neighborSW] == 0)
            {
                tilesForeground[neighborW] = 13;
                tilesBackground[neighborSW] = 12;
            }
            // bottom right inner corner
            else if (oldBackground[neighborS] > 0 && oldBackground[neighborE] > 0 && oldBackground[neighborSE] == 0)
            {
                tilesForeground[neighborE] = 14;
                tilesBackground[neighborSE] = 17;
            }
        }
    }
}