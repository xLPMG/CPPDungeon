#include "Map.hpp"
#include "Generator.hpp"
#include <iostream>
#include <chrono>

cppdungeon::world::Map::Map(u32 seed, u16 width, u16 height, cppdungeon::world::tiles::TileRegistry *tileRegistry)
    : width(width), height(height), tileRegistry(tileRegistry)
{
    auto start = std::chrono::high_resolution_clock::now();
    generator = std::make_unique<cppdungeon::world::Generator>();
    generator->generate(seed, width, height, tilesBackground, tilesForeground, spawnPoint);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Map generation took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
}

cppdungeon::world::Map::~Map()
{
}

void cppdungeon::world::Map::update(float fElapsedTime)
{
}

void cppdungeon::world::Map::renderBackground(olc::PixelGameEngine *pge, olc::vf2d offset, olc::vf2d screenSize)
{
    pge->SetPixelMode(olc::Pixel::MASK);
    olc::vi2d offsetByTiles = offset / TILE_SIZE;
    olc::vi2d start = {std::max(0, (int)offsetByTiles.x), std::max(0, (int)offsetByTiles.y)};
    olc::vi2d end = {std::min(width, (u16)((offset.x + screenSize.x) / TILE_SIZE.x + 1) ), std::min(height, (u16)((offset.y + screenSize.y) / TILE_SIZE.y + 1) )};

    for (int y = start.y; y < end.y; y++)
    {
        for (int x = start.x; x < end.x; x++)
        {
            if (tilesBackground[y * width + x] > 0)
            {
                tileRegistry->tiles[tilesBackground[y * width + x]]->render(pge, olc::vi2d(x, y) * TILE_SIZE - offset);
            }
        }
    }
    pge->SetPixelMode(olc::Pixel::NORMAL);
}

void cppdungeon::world::Map::renderForeground(olc::PixelGameEngine *pge, olc::vf2d offset, olc::vf2d screenSize)
{
    pge->SetPixelMode(olc::Pixel::MASK);
    olc::vi2d offsetByTiles = offset / TILE_SIZE;
    olc::vi2d start = {std::max(0, (int)offsetByTiles.x), std::max(0, (int)offsetByTiles.y)};
    olc::vi2d end = {std::min(width, (u16)((offset.x + screenSize.x) / TILE_SIZE.x + 1)), std::min(height, (u16)((offset.y + screenSize.y) / TILE_SIZE.y + 1))};

    for (int y = start.y; y < end.y; y++)
    {
        for (int x = start.x; x < end.x; x++)
        {
            if (tilesForeground[y * width + x] > 0)
            {
                tileRegistry->tiles[tilesForeground[y * width + x]]->render(pge, olc::vi2d(x, y) * TILE_SIZE - offset);
            }
        }
    }
    pge->SetPixelMode(olc::Pixel::NORMAL);
}

bool cppdungeon::world::Map::collides(olc::vf2d coordinate, cppdungeon::usize &tileId)
{
    if (coordinate.x < 0 || coordinate.y < 0 || coordinate.x >= width * TILE_SIZE.x || coordinate.y >= height * TILE_SIZE.y)
    {
        tileId = 0;
        return true;
    }
    tileId = (u32)(coordinate.x / TILE_SIZE.x) + (u32)(coordinate.y / TILE_SIZE.y) * width;
    return tileRegistry->tiles[tilesBackground[tileId]]->getSolid();
}

void cppdungeon::world::Map::regenerate(u32 seed)
{
    auto start = std::chrono::high_resolution_clock::now();
    generator->generate(seed, width, height, tilesBackground, tilesForeground, spawnPoint);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Map generation took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
}

olc::vf2d cppdungeon::world::Map::getSpawnPoint()
{
    return spawnPoint * TILE_SIZE;
}