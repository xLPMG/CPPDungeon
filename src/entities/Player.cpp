#include "Player.hpp"
#include <iostream>
#include <SDL_image.h>

cppdungeon::entities::Player::Player(SDL_Renderer *renderer, v2 initial_position)
{
    position = initial_position;
    char *pwd = SDL_GetBasePath();
    SDL_Surface *surface;
    surface = IMG_Load("res/textures/hero/hero_right_0001.png");
    rightTexture[0] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = IMG_Load("res/textures/hero/hero_right_0002.png");
    rightTexture[1] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = IMG_Load("res/textures/hero/hero_right_0003.png");
    rightTexture[2] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = IMG_Load("res/textures/hero/hero_right_0004.png");
    rightTexture[3] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = IMG_Load("res/textures/hero/hero_right_0005.png");
    rightTexture[4] = SDL_CreateTextureFromSurface(renderer, surface);

    surface = IMG_Load("res/textures/hero/hero_front_0001.png");
    frontTexture[0] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = IMG_Load("res/textures/hero/hero_front_0002.png");
    frontTexture[1] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = IMG_Load("res/textures/hero/hero_front_0003.png");
    frontTexture[2] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = IMG_Load("res/textures/hero/hero_front_0004.png");
    frontTexture[3] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = IMG_Load("res/textures/hero/hero_front_0005.png");
    frontTexture[4] = SDL_CreateTextureFromSurface(renderer, surface);

    surface = IMG_Load("res/textures/hero/hero_back_0001.png");
    backTexture[0] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = IMG_Load("res/textures/hero/hero_back_0002.png");
    backTexture[1] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = IMG_Load("res/textures/hero/hero_back_0003.png");
    backTexture[2] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = IMG_Load("res/textures/hero/hero_back_0004.png");
    backTexture[3] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = IMG_Load("res/textures/hero/hero_back_0005.png");
    backTexture[4] = SDL_CreateTextureFromSurface(renderer, surface);

    surface = IMG_Load("res/textures/hero/hero_sit_0001.png");
    sitTexture[0] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = IMG_Load("res/textures/hero/hero_sit_0002.png");
    sitTexture[1] = SDL_CreateTextureFromSurface(renderer, surface);
    surface = IMG_Load("res/textures/hero/hero_sit_0003.png");
    sitTexture[2] = SDL_CreateTextureFromSurface(renderer, surface);
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
    SDL_Rect drawRect = {(i32)position.x, (i32)position.y, 64, 64};
    switch (direction)
    {
    case Direction::RIGHT:
        SDL_RenderCopy(renderer, rightTexture[currentRightFrame], NULL, &drawRect);
        break;
    case Direction::LEFT:
        SDL_RenderCopyEx(renderer, rightTexture[currentLeftFrame], NULL, &drawRect, 0, NULL, SDL_FLIP_HORIZONTAL);
        break;
    case Direction::DOWN:
        SDL_RenderCopy(renderer, frontTexture[currentFrontFrame], NULL, &drawRect);
        break;
    case Direction::UP:
        SDL_RenderCopy(renderer, backTexture[currentBackFrame], NULL, &drawRect);
        break;
    default:
        SDL_RenderCopy(renderer, sitTexture[currentSitFrame], NULL, &drawRect);
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
        currentLeftTicks += *deltaTime * (sprinting ? speed * 1.2 : speed);
        if (currentLeftTicks > 10)
        {
            currentLeftTicks = 0;
            currentLeftFrame++;
            if (currentLeftFrame > 4)
            {
                currentLeftFrame = 0;
            }
        }
        direction = Direction::LEFT;
        currentSitFrame = 0;
    }
    else if (actualSpeedX > 0)
    {
        currentRightTicks += *deltaTime * (sprinting ? speed * 1.2 : speed);
        if (currentRightTicks > 10)
        {
            currentRightTicks = 0;
            currentRightFrame++;
            if (currentRightFrame > 4)
            {
                currentRightFrame = 0;
            }
        }
        direction = Direction::RIGHT;
        currentSitFrame = 0;
    }
    else if (actualSpeedY < 0)
    {
        currentBackTicks += *deltaTime * (sprinting ? speed * 1.2 : speed);
        if (currentBackTicks > 10)
        {
            currentBackTicks = 0;
            currentBackFrame++;
            if (currentBackFrame > 4)
            {
                currentBackFrame = 0;
            }
        }
        direction = Direction::UP;
        currentSitFrame = 0;
    }
    else if (actualSpeedY > 0)
    {
        currentFrontTicks += *deltaTime * (sprinting ? speed * 1.2 : speed);
        if (currentFrontTicks > 10)
        {
            currentFrontTicks = 0;
            currentFrontFrame++;
            if (currentFrontFrame > 4)
            {
                currentFrontFrame = 0;
            }
        }
        direction = Direction::DOWN;
        currentSitFrame = 0;
    }
    else
    {
        currentFrontTicks = 0;
        currentFrontFrame = 0;
        currentLeftFrame = 0;
        currentRightFrame = 0;
        currentBackFrame = 0;

        if (currentSitFrame == 0)
        {
            currentSitTicks += *deltaTime * 10;
            if (currentSitTicks > 20)
            {
                currentSitTicks = 0;
                currentSitFrame++;
            }
        }
        else if (currentSitFrame == 1)
        {
            currentSitTicks += *deltaTime * 10;
            if (currentSitTicks > 4)
            {
                currentSitTicks = 0;
                currentSitFrame++;
            }
        }
        direction = Direction::IDLE;
    }

    position.x += actualSpeedX * *deltaTime;
    position.y += actualSpeedY * *deltaTime;
}