#include "Player.hpp"

cppdungeon::entities::Player::Player(v2 initial_position)
{

}

cppdungeon::entities::Player::~Player()
{

}

void cppdungeon::entities::Player::update()
{

}

void cppdungeon::entities::Player::render(u32 *pixels)
{
pixels[size_t(position.x + position.y * SCREEN_WIDTH)] = 0xFF'00'00'FF;
}

void cppdungeon::entities::Player::move(cppdungeon::Direction direction)
{
    switch (direction)
    {
    case Direction::UP:
        position.y += speed;
        break;
    case Direction::DOWN:
        position.y -= speed;
        break;
    case Direction::LEFT:
        position.x -= speed;
        break;
    case Direction::RIGHT:
        position.x += speed;
        break;
    }
}