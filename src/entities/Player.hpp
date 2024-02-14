#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../constants.hpp"
#include <SDL.h>

namespace cppdungeon
{
    namespace entities
    {
        class Player;
    }
}

class cppdungeon::entities::Player
{
private:
    v2 position = {0.0f, 0.0f};
    f32 speed = 100.0f;
    f32 sprintBoost = 2.0f;
    SDL_Texture *rightTexture[5];
    SDL_Texture *frontTexture[5];
    SDL_Texture *backTexture[5];
    SDL_Texture *sitTexture[3];

    u8 currentRightFrame = 0;
    f32 currentRightTicks = 0;
    u8 currentLeftFrame = 0;
    f32 currentLeftTicks = 0;
    u8 currentFrontFrame = 0;
    f32 currentFrontTicks = 0;
    u8 currentBackFrame = 0;
    f32 currentBackTicks = 0;
    u8 currentSitFrame = 0;
    f32 currentSitTicks = 0;

    rect hitbox = {position.x, position.y, 32, 32};

    enum class Direction
    {
        LEFT,
        RIGHT,
        UP,
        DOWN, 
        IDLE
    };

    Direction direction = Direction::RIGHT;

public:
    Player(SDL_Renderer *renderer, v2 initial_position = {0.0f, 0.0f});
    ~Player();
    void update(f32 *deltaTime);
    void render(SDL_Renderer *renderer);
    void move(i8 *x, i8 *y, bool sprinting, f32 *deltaTime);
};

#endif