#include "Map.hpp"

cppdungeon::world::Map::Map(i32 seed, i32 width, i32 height, olc::vf2d tileSize)
    : width(width), height(height), tileSize(tileSize)
{
    tiles = std::make_unique<i32[]>(width * height);
    srand(seed);
    for (i32 y = 0; y < height; y++)
    {
        for (i32 x = 0; x < width; x++)
        {
            if (rand() % 100 < 50)
            {
                tiles[y * width + x] = 1;
            }
            else
            {
                tiles[y * width + x] = 0;
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
    olc::vi2d end = {std::min(width, (int)((offset.x + screenSize.x) / tileSize.x) + 1), std::min(height, (int)((offset.y + screenSize.y) / tileSize.y) +1)};

    for (int y = start.y; y < end.y; y++)
    {
        for (int x = start.x; x < end.x; x++)
        {
            pge->FillRect(olc::vi2d(x, y) * tileSize - offset, tileSize, tiles[y * width + x] == 1 ? olc::WHITE : olc::BLACK);
        }
    }
    pge->SetPixelMode(olc::Pixel::NORMAL);
}

void cppdungeon::world::Map::renderForeground(olc::PixelGameEngine *pge, olc::vf2d offset, olc::vf2d screenSize)
{
}