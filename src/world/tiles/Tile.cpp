#include "Tile.hpp"

using namespace cppdungeon::world::tiles;

Tile::Tile(std::string texture)
{
    this->texture = std::make_unique<olc::Sprite>(texture);
}

Tile::~Tile()
{
}