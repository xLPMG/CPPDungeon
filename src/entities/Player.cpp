#include "Player.hpp"
#include <iostream>
#include <SDL_image.h>

cppdungeon::entities::Player::Player(SDL_Renderer *renderer, v2 initial_position)
{
    position = initial_position;
    SDL_Surface *surface = IMG_Load("res/textures/hero_basic.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

cppdungeon::entities::Player::~Player()
{
}

void cppdungeon::entities::Player::update(f32 *deltaTime)
{
}

void cppdungeon::entities::Player::render(SDL_Renderer *renderer)
{
    if (direction == Direction::RIGHT)
    {
        SDL_RenderCopy(renderer, texture, NULL, &drawRect);
    }
    else
    {
        SDL_RenderCopyEx(renderer, texture, NULL, &drawRect, 0, NULL, SDL_FLIP_HORIZONTAL);
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

    position.x += actualSpeedX * *deltaTime;
    position.y += actualSpeedY * *deltaTime;
}