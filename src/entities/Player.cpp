#include "Player.hpp"

cppdungeon::entities::Player::Player()
{
    position = { 0, 0 };
    size = { 16, 16 };
    idleAnimation = std::make_unique<gfx::Animation>("./res/textures/hero/hero_sit_sheet.png", 16, 16, 0.4, false);
    frontAnimation = std::make_unique<gfx::Animation>("./res/textures/hero/hero_front_sheet.png", 16, 16, 0.15);
    backAnimation = std::make_unique<gfx::Animation>("./res/textures/hero/hero_back_sheet.png", 16, 16, 0.15);
    rightAnimation = std::make_unique<gfx::Animation>("./res/textures/hero/hero_right_sheet.png", 16, 16, 0.15);
    leftAnimation = std::make_unique<gfx::Animation>("./res/textures/hero/hero_right_sheet.png", 16, 16, 0.15);
    leftAnimation->flip(olc::Sprite::HORIZ);
}

cppdungeon::entities::Player::~Player()
{}

void cppdungeon::entities::Player::update(float fElapsedTime)
{
    hitbox = {position.x + bounds.x, position.y + bounds.y, bounds.width, bounds.height};

    switch (direction)
    {
    case Direction::RIGHT:
        rightAnimation->update(fElapsedTime);
        break;
    case Direction::LEFT:
        leftAnimation->update(fElapsedTime);
        break;
    case Direction::DOWN:
        frontAnimation->update(fElapsedTime);
        break;
    case Direction::UP:
        backAnimation->update(fElapsedTime);
        break;
    default:
        idleAnimation->update(fElapsedTime);
        break;
    }
}

void cppdungeon::entities::Player::render(olc::PixelGameEngine* pge, olc::vf2d offset)
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

void cppdungeon::entities::Player::move(i8 *x, i8 *y, bool sprinting, f32 *deltaTime)
{
    f32 actualSpeedX = *x * speed * (sprinting ? sprintBoost : 1);
    f32 actualSpeedY = *y * speed * (sprinting ? sprintBoost : 1);
    if (*x != 0 && *y != 0)
    {
        actualSpeedX *= 0.707;
        actualSpeedY *= 0.707;
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
    }

    position.x += actualSpeedX * *deltaTime;
    position.y += actualSpeedY * *deltaTime;
}