#include "Generator.hpp"
#include <random>
#include <algorithm>
#include <iostream>

#include <delaunator.hpp>
#include <stack>
#include <unordered_set>
#include <iostream>

void cppdungeon::world::Generator::generate(u32 seed, u16 width, u16 height, std::vector<usize> &tilesBackground, std::vector<usize> &tilesForeground, olc::vf2d &spawnPos)
{
    srand(seed);
    this->width = width;
    this->height = height;

    tilesBackground.clear();
    tilesBackground.resize(width * height);
    std::fill(tilesBackground.begin(), tilesBackground.end(), 0);

    tilesForeground.clear();
    tilesForeground.resize(width * height);
    std::fill(tilesForeground.begin(), tilesForeground.end(), 0);

    bounds.pos = {2, 2};
    bounds.size = {width - 4, height - 4};

    tilesBackgroundPtr = &tilesBackground;
    tilesForegroundPtr = &tilesForeground;

    /////////////////////
    // ROOM GENERATION //
    /////////////////////
    generateRooms();
    olc::utils::geom2d::rect<i32> spawnRoom = rooms[rand() % rooms.size()];
    spawnPos = spawnRoom.middle();

    /////////////////////
    // PATH GENERATION //
    /////////////////////
    std::map<olc::vi2d, std::vector<olc::vi2d>> graph;
    constructDelauneyTriangles(graph);

    std::vector<std::pair<olc::vi2d, olc::vi2d>> spanningTree;
    dfsSpanningTree(graph, graph.begin()->first, spanningTree);

    for (auto &edge : spanningTree)
    {
        olc::vi2d start = edge.first;
        olc::vi2d end = edge.second;
        connectPoints(start, end);
    }

    ////////////////////
    //   FILL HOLES   //
    ////////////////////

    /**
     * The map will contain weird gaps, such as 1 or 2 tile wide spaces between corridors.
     * The issue with that is, that wall textures will be rendered over each other, and the map will look weird.
     * To fix this, we will fill the gaps with floor tiles and just expand the walkable area.
     * */
    fillGaps();

    ////////////////////
    // MAP DECORATION //
    ////////////////////
    buildWalls();
    decorateFloor();
}

void cppdungeon::world::Generator::generateRooms()
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
                    carve(olc::vi2d(x, y), 1, *tilesBackgroundPtr);
                }
            }
        }
    }
}

void cppdungeon::world::Generator::connectPoints(olc::vi2d p1, olc::vi2d p2)
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
                carve({p1.x - 1, p1.y + i * signY}, 1, *tilesBackgroundPtr);
                carve({p1.x, p1.y + i * signY}, 1, *tilesBackgroundPtr);
                carve({p1.x + 1, p1.y + i * signY}, 1, *tilesBackgroundPtr);
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
                carve({p1.x + i * signX, p1.y - 1}, 1, *tilesBackgroundPtr);
                carve({p1.x + i * signX, p1.y}, 1, *tilesBackgroundPtr);
                carve({p1.x + i * signX, p1.y + 1}, 1, *tilesBackgroundPtr);
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
                carve({p1.x + i * signX, p1.y - 1}, 1, *tilesBackgroundPtr);
                carve({p1.x + i * signX, p1.y}, 1, *tilesBackgroundPtr);
                carve({p1.x + i * signX, p1.y + 1}, 1, *tilesBackgroundPtr);
            }

            // Then connect vertically towards p2
            int dy = abs(p2.y - p1.y);
            int signY = (p2.y - p1.y > 0) ? 1 : -1;

            for (int i = 0; i <= dy; ++i)
            {
                carve({p2.x - 1, p1.y + i * signY}, 1, *tilesBackgroundPtr);
                carve({p2.x, p1.y + i * signY}, 1, *tilesBackgroundPtr);
                carve({p2.x + 1, p1.y + i * signY}, 1, *tilesBackgroundPtr);
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
                carve({p1.x - 1, p1.y + i * signY}, 1, *tilesBackgroundPtr);
                carve({p1.x, p1.y + i * signY}, 1, *tilesBackgroundPtr);
                carve({p1.x + 1, p1.y + i * signY}, 1, *tilesBackgroundPtr);
            }
            // Then connect horizontally towards p2
            int dx = abs(p2.x - p1.x);
            int signX = (p2.x - p1.x > 0) ? 1 : -1;

            for (int i = 0; i <= dx; ++i)
            {
                carve({p1.x + i * signX, p2.y - 1}, 1, *tilesBackgroundPtr);
                carve({p1.x + i * signX, p2.y}, 1, *tilesBackgroundPtr);
                carve({p1.x + i * signX, p2.y + 1}, 1, *tilesBackgroundPtr);
            }
        }
    }
}

void cppdungeon::world::Generator::carve(olc::vi2d pos, u16 tile, std::vector<usize> &tiles)
{
    tiles.at(pos.y * width + pos.x) = tile;
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

void cppdungeon::world::Generator::fillGaps()
{
    for (i32 x = bounds.pos.x; x < bounds.pos.x + bounds.size.x; x++)
    {
        for (i32 y = bounds.pos.y; y < bounds.pos.y + bounds.size.y; y++)
        {
            i32 self = idx(x, y, width);
            if (tilesBackgroundPtr->at(self) == 0)
            {
                // one tile in each direction
                if (tilesBackgroundPtr->at(idx(x + 1, y, width)) > 0 && tilesBackgroundPtr->at(idx(x - 1, y, width)) > 0)
                {
                    tilesBackgroundPtr->at(self) = 1;
                }
                else if (tilesBackgroundPtr->at(idx(x, y + 1, width)) > 0 && tilesBackgroundPtr->at(idx(x, y - 1, width)) > 0)
                {
                    tilesBackgroundPtr->at(self) = 1;
                }

                // one and two tiles
                else if (tilesBackgroundPtr->at(idx(x, y + 2, width)) > 0 && tilesBackgroundPtr->at(idx(x, y - 1, width)) > 0)
                {
                    tilesBackgroundPtr->at(self) = 1;
                }
                else if (tilesBackgroundPtr->at(idx(x, y + 1, width)) > 0 && tilesBackgroundPtr->at(idx(x, y - 2, width)) > 0)
                {
                    tilesBackgroundPtr->at(self) = 1;
                }
                else if (tilesBackgroundPtr->at(idx(x + 2, y, width)) > 0 && tilesBackgroundPtr->at(idx(x - 1, y, width)) > 0)
                {
                    tilesBackgroundPtr->at(self) = 1;
                }
                else if (tilesBackgroundPtr->at(idx(x + 1, y, width)) > 0 && tilesBackgroundPtr->at(idx(x - 2, y, width)) > 0)
                {
                    tilesBackgroundPtr->at(self) = 1;
                }
            }
        }
    }

    // sometimes the filling process leave 1x1 holes. This should fix it.
    std::vector<usize> oldBackground = *tilesBackgroundPtr;
    for (i32 x = bounds.pos.x; x < bounds.pos.x + bounds.size.x; x++)
    {
        for (i32 y = bounds.pos.y; y < bounds.pos.y + bounds.size.y; y++)
        {
            i32 self = idx(x, y, width);
            if (oldBackground.at(self) == 0)
            {
                // one tile in each direction
                if (oldBackground.at(idx(x + 1, y, width)) > 0 && oldBackground.at(idx(x - 1, y, width)) > 0)
                {
                    tilesBackgroundPtr->at(self) = 1;
                }
                else if (oldBackground.at(idx(x, y + 1, width)) > 0 && oldBackground.at(idx(x, y - 1, width)) > 0)
                {
                    tilesBackgroundPtr->at(self) = 1;
                }
            }
        }
    }
}

void cppdungeon::world::Generator::buildWalls()
{

    std::vector<usize> oldBackground = *tilesBackgroundPtr;

    for (i32 y = bounds.pos.y; y < bounds.pos.y + bounds.size.y; y++)
    {
        for (i32 x = bounds.pos.x; x < bounds.pos.x + bounds.size.x; x++)
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
                tilesBackgroundPtr->at(neighborN) = 2;
                tilesBackgroundPtr->at(idx(pos + N + N, width)) = 5;
            }
            // bottom middle wall
            if (oldBackground[neighborS] == 0 && oldBackground[neighborSW] == 0 && oldBackground[neighborSE] == 0 && oldBackground[neighborW] > 0 && oldBackground[neighborE] > 0)
            {
                tilesBackgroundPtr->at(neighborS) = 18;
                tilesForegroundPtr->at(self) = 19;
            }
            // left center wall
            else if (oldBackground[neighborW] == 0 && oldBackground[neighborNW] == 0 && oldBackground[neighborSW] == 0 && oldBackground[neighborN] > 0 && oldBackground[neighborS] > 0)
            {
                tilesBackgroundPtr->at(neighborW) = rand() % 2 == 0 ? 10 : 11;
            }
            // right center wall
            else if (oldBackground[neighborE] == 0 && oldBackground[neighborNE] == 0 && oldBackground[neighborSE] == 0 && oldBackground[neighborN] > 0 && oldBackground[neighborS] > 0)
            {
                tilesBackgroundPtr->at(neighborE) = rand() % 2 == 0 ? 3 : 4;
            }
            // top left outer corner
            else if (oldBackground[neighborN] == 0 && oldBackground[neighborW] == 0 && oldBackground[neighborNW] == 0)
            {
                if (oldBackground[neighborNE] == 0)
                {
                    tilesBackgroundPtr->at(neighborN) = 2;
                    tilesBackgroundPtr->at(idx(pos + N + N, width)) = 5;
                }
                else
                {
                    tilesBackgroundPtr->at(neighborN) = 21;
                    tilesBackgroundPtr->at(idx(pos + N + N, width)) = 15;
                }

                tilesBackgroundPtr->at(neighborW) = rand() % 2 == 0 ? 10 : 11;
                tilesBackgroundPtr->at(neighborNW) = rand() % 2 == 0 ? 10 : 11;
                tilesBackgroundPtr->at(idx(pos + N + N + W, width)) = 7;
            }
            // top right outer corner
            else if (oldBackground[neighborN] == 0 && oldBackground[neighborE] == 0 && oldBackground[neighborNE] == 0)
            {
                if (oldBackground[neighborNW] == 0)
                {
                    tilesBackgroundPtr->at(neighborN) = 2;
                    tilesBackgroundPtr->at(idx(pos + N + N, width)) = 5;
                }
                else
                {
                    tilesBackgroundPtr->at(neighborN) = 20;
                    tilesBackgroundPtr->at(idx(pos + N + N, width)) = 16;
                }

                tilesBackgroundPtr->at(neighborE) = rand() % 2 == 0 ? 3 : 4;
                tilesBackgroundPtr->at(neighborNE) = rand() % 2 == 0 ? 3 : 4;
                tilesBackgroundPtr->at(idx(pos + N + N + E, width)) = 6;
            }
            // bottom left outer corner
            else if (oldBackground[neighborS] == 0 && oldBackground[neighborW] == 0 && oldBackground[neighborSW] == 0)
            {
                tilesBackgroundPtr->at(neighborS) = 18;
                tilesForegroundPtr->at(self) = 19;

                if (oldBackground[neighborNW] == 0)
                {
                    tilesBackgroundPtr->at(neighborSW) = 9;
                    tilesBackgroundPtr->at(neighborW) = 10;
                }
                else
                {
                    tilesBackgroundPtr->at(neighborSW) = 9;
                    tilesBackgroundPtr->at(neighborW) = 12;
                }
            }
            // bottom right outer corner
            else if (oldBackground[neighborS] == 0 && oldBackground[neighborE] == 0 && oldBackground[neighborSE] == 0)
            {
                // TODO: do this for all other outer corners
                if (oldBackground[neighborSW] == 0 && oldBackground[neighborNE] == 0)
                {
                    tilesBackgroundPtr->at(neighborS) = 18;
                    tilesForegroundPtr->at(self) = 19;
                    tilesBackgroundPtr->at(neighborE) = 3;
                }
                else if (oldBackground[neighborSW] > 0 && oldBackground[neighborNE] == 0)
                {
                    tilesBackgroundPtr->at(neighborS) = 17;
                    tilesForegroundPtr->at(self) = 14;
                    tilesBackgroundPtr->at(neighborE) = 3;
                }
                else if (oldBackground[neighborSW] == 0 && oldBackground[neighborNE] > 0)
                {
                    tilesBackgroundPtr->at(neighborS) = 18;
                    tilesForegroundPtr->at(self) = 14;
                    tilesBackgroundPtr->at(neighborE) = 17;
                }
                else
                {
                    tilesForegroundPtr->at(self) = 14;
                    tilesBackgroundPtr->at(neighborE) = 17;
                    tilesBackgroundPtr->at(neighborS) = 17;
                }

                tilesBackgroundPtr->at(neighborSE) = 8;
            }
            // top left inner corner
            else if (oldBackground[neighborN] > 0 && oldBackground[neighborW] > 0 && oldBackground[neighborNW] == 0)
            {
                tilesBackgroundPtr->at(neighborNW) = 21;
                tilesBackgroundPtr->at(idx(pos + N + N + W, width)) = 15;
            }
            // top right inner corner
            else if (oldBackground[neighborN] > 0 && oldBackground[neighborE] > 0 && oldBackground[neighborNE] == 0)
            {
                tilesBackgroundPtr->at(neighborNE) = 20;
                tilesBackgroundPtr->at(idx(pos + N + N + E, width)) = 16;
            }
            // bottom left inner corner
            else if (oldBackground[neighborS] > 0 && oldBackground[neighborW] > 0 && oldBackground[neighborSW] == 0)
            {
                tilesForegroundPtr->at(neighborW) = 13;
                tilesBackgroundPtr->at(neighborSW) = 12;
            }
            // bottom right inner corner
            else if (oldBackground[neighborS] > 0 && oldBackground[neighborE] > 0 && oldBackground[neighborSE] == 0)
            {
                tilesForegroundPtr->at(neighborE) = 14;
                tilesBackgroundPtr->at(neighborSE) = 17;
            }
        }
    }
}

void cppdungeon::world::Generator::decorateFloor()
{
    f32 chanceMudPatch = 5;
    f32 chanceStain = 30;

    for (i32 y = bounds.pos.y; y < bounds.pos.y + bounds.size.y; y++)
    {
        for (i32 x = bounds.pos.x; x < bounds.pos.x + bounds.size.x; x++)
        {
            if (tilesBackgroundPtr->at(idx(x, y, width)) != 1)
                continue;

            if (rand() % 100 < chanceMudPatch)
            {
                bool canSpawn = true;
                i32 w = rand() % 4 + 7;
                i32 h = rand() % 4 + 7;

                for (i32 j = y; j < y + h; j++)
                {
                    for (i32 i = x; i < x + w; i++)
                    {
                        if (!canSpawn)
                            break;

                        if (tilesBackgroundPtr->at(idx(i, j, width)) != 1)
                        {
                            canSpawn = false;
                        }
                    }
                }

                if (canSpawn)
                {
                    olc::vi2d pos = {x + 2, y + 2};
                    w -= 4;
                    h -= 4;

                    // CORNERS
                    tilesBackgroundPtr->at(idx(pos.x + w, pos.y, width)) = 26;     // NE
                    tilesBackgroundPtr->at(idx(pos.x, pos.y, width)) = 27;         // NW
                    tilesBackgroundPtr->at(idx(pos.x + w, pos.y + h, width)) = 30; // SE
                    tilesBackgroundPtr->at(idx(pos.x, pos.y + h, width)) = 31;     // SW

                    // VERTICAL EDGES
                    for (i32 j = pos.y + 1; j < pos.y + h; j++)
                    {
                        tilesBackgroundPtr->at(idx(pos.x, j, width)) = 32;
                        tilesBackgroundPtr->at(idx(pos.x + w, j, width)) = 33;
                    }
                    // HORIZONTAL EDGES
                    for (i32 i = pos.x + 1; i < pos.x + w; i++)
                    {
                        tilesBackgroundPtr->at(idx(i, pos.y, width)) = rand() % 2 == 0 ? 24 : 25;
                        tilesBackgroundPtr->at(idx(i, pos.y + h, width)) = rand() % 2 == 0 ? 28 : 29;
                    }

                    // MIDDLE
                    for (i32 j = pos.y + 1; j < pos.y + h; j++)
                    {
                        for (i32 i = pos.x + 1; i < pos.x + w; i++)
                        {
                            tilesBackgroundPtr->at(idx(i, j, width)) = rand() % 2 == 0 ? 22 : 23;
                        }
                    }
                }
            }
        }
    }
}