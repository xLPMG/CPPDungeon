#include "TinyZombie.hpp"

cppdungeon::entities::TinyZombie::TinyZombie(olc::vf2d position) : Entity(position, {16, 16})
{
    idleAnimation = std::make_unique<gfx::Animation>("./res/textures/creatures/tinyZombie/tiny_zombie_idle.png", 16, 16, 0.4);
    rightAnimation = std::make_unique<gfx::Animation>("./res/textures/creatures/tinyZombie/tiny_zombie_run.png", 16, 16, 0.2);
    leftAnimation = std::make_unique<gfx::Animation>("./res/textures/creatures/tinyZombie/tiny_zombie_run.png", 16, 16, 0.2);
    leftAnimation->flip(olc::Sprite::HORIZ);
}

cppdungeon::entities::TinyZombie::~TinyZombie()
{
}

void cppdungeon::entities::TinyZombie::update(float &deltaTime, cppdungeon::world::Map *map)
{
    hitbox = {position.x + bounds.x, position.y + bounds.y, bounds.width, bounds.height};
    bool noX = true;
    bool noY = true;
    if (followTarget != nullptr)
    {
        if (position.x < followTarget->getPosition().x - followRange)
        {
            position.x += speed * deltaTime;
            direction = Direction::RIGHT;
            noX = false;
        }
        else if (position.x > followTarget->getPosition().x + followRange)
        {
            position.x -= speed * deltaTime;
            direction = Direction::LEFT;
            noX = false;
        }

        if (position.y < followTarget->getPosition().y - followRange)
        {
            position.y += speed * deltaTime;
            direction = Direction::RIGHT;
            noY = false;
        }
        else if (position.y > followTarget->getPosition().y + followRange)
        {
            position.y -= speed * deltaTime;
            direction = Direction::LEFT;
            noY = false;
        }
    }

    if (noX && noY)
    {
        direction = Direction::IDLE;
    }
    else
    {
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

    switch (direction)
    {
    case Direction::RIGHT:
        rightAnimation->update(deltaTime);
        break;
    case Direction::LEFT:
        leftAnimation->update(deltaTime);
        break;
    default:
        idleAnimation->update(deltaTime);
        break;
    }
}

void cppdungeon::entities::TinyZombie::render(olc::PixelGameEngine *pge, olc::vf2d &offset)
{
    switch (direction)
    {
    case Direction::IDLE:
        idleAnimation->render(pge, position - offset);
        break;
    case Direction::LEFT:
        leftAnimation->render(pge, position - offset);
        break;
    case Direction::RIGHT:
        rightAnimation->render(pge, position - offset);
        break;
    default:
        idleAnimation->render(pge, position - offset);
        break;
    }
}

void cppdungeon::entities::TinyZombie::followWithin(cppdungeon::entities::Entity *target, f32 distance)
{
    this->followTarget = target;
    this->followRange = distance;
}