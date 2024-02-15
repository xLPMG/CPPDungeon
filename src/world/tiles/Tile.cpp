#include "Tile.hpp"

using namespace cppdungeon::world::tiles;

Tile::Tile(std::string texture)
{
    this->texture = std::make_unique<olc::Sprite>(texture);
}

Tile::~Tile()
{
}

void Tile::render(olc::PixelGameEngine* pge, olc::vf2d position)
{
    pge->DrawSprite(position, texture.get());
}