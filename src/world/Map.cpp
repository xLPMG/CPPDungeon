#include "Map.hpp"
#include "Generator.hpp"
#include <iostream>
#include <chrono>

cppdungeon::world::Map::Map(i32 seed, i32 width, i32 height, olc::vf2d tileSize, cppdungeon::world::tiles::TileRegistry *tileRegistry)
    : width(width), height(height), tileSize(tileSize), tileRegistry(tileRegistry)
{
    auto start = std::chrono::high_resolution_clock::now();
    generator = new cppdungeon::world::Generator();
    generator->generate(seed, width, height, tilesBackground, tilesForeground, spawnPoint);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Map generation took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
}

cppdungeon::world::Map::~Map()
{
    delete generator;
}

void cppdungeon::world::Map::update(float fElapsedTime)
{
}

void cppdungeon::world::Map::renderBackground(olc::PixelGameEngine *pge, olc::vf2d offset, olc::vf2d screenSize)
{
    pge->SetPixelMode(olc::Pixel::MASK);
    olc::vi2d offsetByTiles = offset / tileSize;
    olc::vi2d start = {std::max(0, (int)offsetByTiles.x), std::max(0, (int)offsetByTiles.y)};
    olc::vi2d end = {std::min(width, (int)((offset.x + screenSize.x) / tileSize.x) + 1), std::min(height, (int)((offset.y + screenSize.y) / tileSize.y) + 1)};

    for (int y = start.y; y < end.y; y++)
    {
        for (int x = start.x; x < end.x; x++)
        {
            if (tilesBackground[y * width + x] > 0)
            {
                tileRegistry->tiles[tilesBackground[y * width + x]]->render(pge, olc::vi2d(x, y) * tileSize - offset);
            }
        }
    }
    pge->SetPixelMode(olc::Pixel::NORMAL);
}

void cppdungeon::world::Map::renderForeground(olc::PixelGameEngine *pge, olc::vf2d offset, olc::vf2d screenSize)
{
    pge->SetPixelMode(olc::Pixel::MASK);
    olc::vi2d offsetByTiles = offset / tileSize;
    olc::vi2d start = {std::max(0, (int)offsetByTiles.x), std::max(0, (int)offsetByTiles.y)};
    olc::vi2d end = {std::min(width, (int)((offset.x + screenSize.x) / tileSize.x) + 1), std::min(height, (int)((offset.y + screenSize.y) / tileSize.y) + 1)};

    for (int y = start.y; y < end.y; y++)
    {
        for (int x = start.x; x < end.x; x++)
        {
            if (tilesForeground[y * width + x] > 0)
            {
                tileRegistry->tiles[tilesForeground[y * width + x]]->render(pge, olc::vi2d(x, y) * tileSize - offset);
            }
        }
    }
    pge->SetPixelMode(olc::Pixel::NORMAL);
}

bool cppdungeon::world::Map::collides(olc::vf2d coordinate, cppdungeon::u32 &tileId)
{
    if (coordinate.x < 0 || coordinate.y < 0 || coordinate.x >= width * tileSize.x || coordinate.y >= height * tileSize.y)
    {
        tileId = 0;
        return true;
    }
    tileId = (u32)(coordinate.x / tileSize.x) + (u32)(coordinate.y / tileSize.y) * width;
    return tileRegistry->tiles[tilesBackground[tileId]]->getSolid();
}

void cppdungeon::world::Map::regenerate(i32 seed)
{
    generator->generate(seed, width, height, tilesBackground, tilesForeground, spawnPoint);
}

olc::vf2d cppdungeon::world::Map::getSpawnPoint()
{
    return spawnPoint* tileSize;
}