#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../constants.hpp"
#include <SDL.h>

namespace cppdungeon {
    namespace entities {
        class Player;
    }
}

class cppdungeon::entities::Player
{
private:
    v2 position = {0.0f, 0.0f};
    f32 speed = 10.0f;
    f32 sprintBoost = 2.0f;
    SDL_Texture *texture;
    SDL_Rect drawRect = {(i32)position.x, (i32)position.y, 32, 32};
    rect hitbox = {position.x, position.y, 32, 32};

    enum class Direction
    {
        LEFT,
        RIGHT
    };

    Direction direction = Direction::RIGHT;

public:
    Player(SDL_Renderer *renderer, v2 initial_position = {0.0f, 0.0f});
    ~Player();
    void update(f32 *deltaTime);
    void render(SDL_Renderer *renderer);
    void move(i8 *x, i8* y, bool sprinting, f32 *deltaTime);
};

#endif