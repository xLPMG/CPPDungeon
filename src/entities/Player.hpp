#ifndef CPPDUNGEON_ENTITIES_PLAYER_HPP
#define CPPDUNGEON_ENTITIES_PLAYER_HPP

#include "Entity.hpp"
#include "../gfx/Animation.hpp"

namespace cppdungeon
{
    namespace entities
    {
        class Player;
    }
}

class cppdungeon::entities::Player : public Entity
{
private:
    std::unique_ptr<gfx::Animation> idleAnimation;
    std::unique_ptr<gfx::Animation> frontAnimation;
    std::unique_ptr<gfx::Animation> backAnimation;
    std::unique_ptr<gfx::Animation> leftAnimation;
    std::unique_ptr<gfx::Animation> rightAnimation;

    rect bounds = {3, 7, 10, 9};
    rect hitbox = {0,0,0,0};
    enum class Direction
    {
        LEFT,
        RIGHT,
        UP,
        DOWN,
        IDLE
    };
    Direction direction = Direction::IDLE;

public:
    Player();
    ~Player();
    void update(float fElapsedTime);
    void render(olc::PixelGameEngine *pge, olc::vf2d offset);
    void move(i8 *x, i8 *y, bool sprinting, f32 *elapsedTime);
};

#endif