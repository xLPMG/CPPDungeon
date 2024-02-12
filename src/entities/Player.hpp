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
    f32 speed = 1.0f;

public:
    Player(v2 initial_position = {0.0f, 0.0f});
    ~Player();
    void update();
    void render(u32 *pixels);
    void move(Direction direction);
};

#endif