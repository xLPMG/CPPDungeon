/**
 * @file Player.cpp
 *
 * @brief This file implements the Player class.
 *
 * @author Luca-Philipp Grumbach
 */
#ifndef CPPDUNGEON_ENTITIES_PLAYER_HPP
#define CPPDUNGEON_ENTITIES_PLAYER_HPP

#include "Entity.hpp"
#include "../gfx/Animation.hpp"
#include "../gameobjects/Inventory.hpp"

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
    std::unique_ptr<cppdungeon::gameobjects::Inventory> inventory;

    rect bounds = {3, 10, 10, 6};
    rect hitbox = {0, 0, 0, 0};
    Direction direction = Direction::IDLE;

public:
    Player(olc::vf2d position = {0, 0});
    ~Player();
    void update(float &deltaTime, cppdungeon::world::Map *map);
    void render(olc::PixelGameEngine *pge, olc::vf2d &offset);
    void renderInventory(olc::PixelGameEngine *pge);
    void move(i8 &x, i8 &y, bool sprinting, f32 &deltaTime, cppdungeon::world::Map *map);
};

#endif