#include "Map.hpp"

cppdungeon::world::Map::Map(i32 seed, i32 width, i32 height, olc::vf2d tileSize, cppdungeon::world::tiles::TileRegistry *tileRegistry)
    : width(width), height(height), tileSize(tileSize), tileRegistry(tileRegistry)
{
    tiles = new i32[width * height]{0};

    srand(seed);
    for (i32 y = 0; y < height; y++)
    {
        for (i32 x = 0; x < width; x++)
        {
            if (rand() % 100 < 50)
            {
                tiles[y * width + x] = 1;
            }
        }
    }
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
            if (tiles[y * width + x] > 0)
            {
                tileRegistry->tiles[tiles[y * width + x]]->render(pge, olc::vi2d(x, y) * tileSize - offset);
            }
        }
    }
    pge->SetPixelMode(olc::Pixel::NORMAL);
}

void cppdungeon::world::Map::renderForeground(olc::PixelGameEngine *pge, olc::vf2d offset, olc::vf2d screenSize)
{
}

bool cppdungeon::world::Map::collides(olc::vf2d coordinate)
{
    if(coordinate.x < 0 || coordinate.y < 0 || coordinate.x >= width * tileSize.x || coordinate.y >= height * tileSize.y)
    {
        return true;
    }
    return tileRegistry->tiles[tiles[(u32)(coordinate.x / tileSize.x) + (u32)(coordinate.y / tileSize.y) * width]]->getSolid();
}