#include "Map.hpp"
#include "Generator.hpp"
#include <iostream>
#include <chrono>

cppdungeon::world::Map::Map(u32 seed, u32 width, u32 height, cppdungeon::world::tiles::TileRegistry *tileRegistry)
    : tileRegistry(tileRegistry)
{
    mapSize = {width, height};

    auto start = std::chrono::high_resolution_clock::now();
    generator = std::make_unique<cppdungeon::world::Generator>();
    generator->generate(seed, width, height, tilesBackground, tilesForeground, mapInfo);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Map generation took " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()/1000 << " ms" << std::endl;
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
    olc::vf2d offsetByTiles = offset / TILE_SIZE;
    olc::vu2d start = {(u32)std::max(0.0f, offsetByTiles.x), (u32)std::max(0.0f, offsetByTiles.y)};
    olc::vu2d end = {std::min(mapSize.x, (u32)((offset.x + screenSize.x) / TILE_SIZE.x + 1) ), std::min(mapSize.y, (u32)((offset.y + screenSize.y) / TILE_SIZE.y + 1) )};

    for (u32 y = start.y; y < end.y; y++)
    {
        for (u32 x = start.x; x < end.x; x++)
        {
            if (tilesBackground[y * mapSize.x + x] > 0)
            {
                tileRegistry->tiles[tilesBackground[y * mapSize.x + x]]->render(pge, olc::vi2d(x, y) * TILE_SIZE - offset);
            }
        }
    }
    pge->SetPixelMode(olc::Pixel::NORMAL);
}

void cppdungeon::world::Map::renderForeground(olc::PixelGameEngine *pge, olc::vf2d offset, olc::vf2d screenSize)
{
    pge->SetPixelMode(olc::Pixel::MASK);
    olc::vf2d offsetByTiles = offset / TILE_SIZE;
    olc::vu2d start = {(u32)std::max(0.0f, offsetByTiles.x), (u32)std::max(0.0f, offsetByTiles.y)};
    olc::vu2d end = {std::min(mapSize.x, (u32)((offset.x + screenSize.x) / TILE_SIZE.x + 1) ), std::min(mapSize.y, (u32)((offset.y + screenSize.y) / TILE_SIZE.y + 1) )};

    for (u32 y = start.y; y < end.y; y++)
    {
        for (u32 x = start.x; x < end.x; x++)
        {
            if (tilesForeground[y * mapSize.x + x] > 0)
            {
                tileRegistry->tiles[tilesForeground[y * mapSize.x + x]]->render(pge, olc::vi2d(x, y) * TILE_SIZE - offset);
            }
        }
    }
    pge->SetPixelMode(olc::Pixel::NORMAL);
}

bool cppdungeon::world::Map::collides(olc::vf2d coordinate, cppdungeon::usize &tileId)
{
    if (coordinate.x < 0 || coordinate.y < 0 || coordinate.x >= mapSize.x * TILE_SIZE.x || coordinate.y >= mapSize.y * TILE_SIZE.y)
    {
        tileId = 0;
        return true;
    }
    tileId = (u32)(coordinate.x / TILE_SIZE.x) + (u32)(coordinate.y / TILE_SIZE.y) * mapSize.x;
    return tileRegistry->tiles[tilesBackground[tileId]]->getSolid();
}

void cppdungeon::world::Map::regenerate(u32 seed)
{
    auto start = std::chrono::high_resolution_clock::now();
    generator->generate(seed, mapSize.x, mapSize.y, tilesBackground, tilesForeground, mapInfo);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Map generation took " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()/1000 << " ms" << std::endl;
}

olc::vf2d cppdungeon::world::Map::getSpawnPoint()
{
    return mapInfo.spawnPoint * TILE_SIZE;
}

cppdungeon::usize cppdungeon::world::Map::getTileID(olc::vf2d position)
{
    return tilesBackground.at((u32)(position.x / TILE_SIZE.x) + (u32)(position.y / TILE_SIZE.y) * mapSize.x);
}