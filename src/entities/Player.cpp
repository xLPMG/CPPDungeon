#include "Player.hpp"
#include <iostream>

cppdungeon::entities::Player::Player(olc::vf2d position) : Entity(position, {16, 16})
{
    idleAnimation = std::make_unique<gfx::Animation>("./res/textures/hero/hero_sit_sheet.png", 16, 16, 0.4, false);
    frontAnimation = std::make_unique<gfx::Animation>("./res/textures/hero/hero_front_sheet.png", 16, 16, 0.15);
    backAnimation = std::make_unique<gfx::Animation>("./res/textures/hero/hero_back_sheet.png", 16, 16, 0.15);
    rightAnimation = std::make_unique<gfx::Animation>("./res/textures/hero/hero_right_sheet.png", 16, 16, 0.15);
    leftAnimation = std::make_unique<gfx::Animation>("./res/textures/hero/hero_right_sheet.png", 16, 16, 0.15);
    leftAnimation->flip(olc::Sprite::HORIZ);
    inventory = std::make_unique<cppdungeon::items::Inventory>();
}

cppdungeon::entities::Player::~Player()
{
}

void cppdungeon::entities::Player::update(float &deltaTime, cppdungeon::world::Map *map)
{
    hitbox = {position.x + bounds.x, position.y + bounds.y, bounds.width, bounds.height};

    switch (direction)
    {
    case Direction::RIGHT:
        rightAnimation->update(deltaTime);
        break;
    case Direction::LEFT:
        leftAnimation->update(deltaTime);
        break;
    case Direction::DOWN:
        frontAnimation->update(deltaTime);
        break;
    case Direction::UP:
        backAnimation->update(deltaTime);
        break;
    default:
        idleAnimation->update(deltaTime);
        break;
    }
}

void cppdungeon::entities::Player::renderInventory(olc::PixelGameEngine *pge)
{
    inventory->render(pge);
}

void cppdungeon::entities::Player::render(olc::PixelGameEngine *pge, olc::vf2d &offset)
{
    switch (direction)
    {
    case Direction::RIGHT:
        rightAnimation->render(pge, position - offset);
        break;
    case Direction::LEFT:
        leftAnimation->render(pge, position - offset);
        break;
    case Direction::DOWN:
        frontAnimation->render(pge, position - offset);
        break;
    case Direction::UP:
        backAnimation->render(pge, position - offset);
        break;
    default:
        idleAnimation->render(pge, position - offset);
        break;
    }
}

void cppdungeon::entities::Player::move(i8 &x, i8 &y, bool sprinting, f32 &deltaTime, cppdungeon::world::Map *map)
{
    f32 actualSpeedX = x * speed * (sprinting ? sprintBoost : 1);
    f32 actualSpeedY = y * speed * (sprinting ? sprintBoost : 1);
    if (x != 0 && y != 0)
    {
        actualSpeedX *= 0.707;
        actualSpeedY *= 0.707;
    }
    // stairs
    if (map->getTileID(position) >= 44 && map->getTileID(position) <= 49)
    {
        actualSpeedX *= 0.5;
        actualSpeedY *= 0.5;
    }

    if (actualSpeedX < 0)
    {
        direction = Direction::LEFT;
    }
    else if (actualSpeedX > 0)
    {
        direction = Direction::RIGHT;
    }
    else if (actualSpeedY < 0)
    {
        direction = Direction::UP;
    }
    else if (actualSpeedY > 0)
    {
        direction = Direction::DOWN;
    }
    else
    {
        direction = Direction::IDLE;
        return;
    }

    position += olc::vf2d(actualSpeedX, actualSpeedY) * deltaTime;

    usize tileId;
    // top middle
    if (map->collides({position.x + bounds.x + bounds.width / 2, position.y + bounds.y}, tileId))
    {
        position.y = (u32)(tileId / map->getWidth() + 1) * TILE_SIZE.y - bounds.y;
        // bottom middle
    }
    else if (map->collides({position.x + bounds.x + bounds.width / 2, position.y + bounds.y + bounds.height}, tileId))
    {
        position.y = (u32)(tileId / map->getWidth()) * TILE_SIZE.y - bounds.y - bounds.height;
    }

    // left middle
    if (map->collides({position.x + bounds.x, position.y + bounds.y + bounds.height / 2}, tileId))
    {
        position.x = (u32)(tileId % map->getWidth() + 1) * TILE_SIZE.x - bounds.x;
        // right middle
    }
    else if (map->collides({position.x + bounds.x + bounds.width, position.y + bounds.y + bounds.height / 2}, tileId))
    {
        position.x = (u32)(tileId % map->getWidth()) * TILE_SIZE.x - bounds.x - bounds.width;
    }
}

cppdungeon::items::Inventory *cppdungeon::entities::Player::getInventory()
{
    return inventory.get();
}