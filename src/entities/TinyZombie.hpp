/** 
 * @brief TinyZombie entity class.
 *
 * @author Luca-Philipp Grumbach
 */
#ifndef CPPDUNGEON_ENTITIES_TINY_ZOMBIE_HPP
#define CPPDUNGEON_ENTITIES_TINY_ZOMBIE_HPP

#include "Entity.hpp"
#include "../gfx/Animation.hpp"

namespace cppdungeon
{
    namespace entities
    {
        class TinyZombie;
    }
}

class cppdungeon::entities::TinyZombie : public Entity
{
private:
    std::unique_ptr<gfx::Animation> idleAnimation;
    std::unique_ptr<gfx::Animation> leftAnimation;
    std::unique_ptr<gfx::Animation> rightAnimation;

    rect bounds = {0, 0, 16, 16};
    rect hitbox = {0, 0, 0, 0};

    Direction direction = Direction::IDLE;

public:
    TinyZombie(olc::vf2d position = {0, 0});
    ~TinyZombie();
    void update(float &deltaTime, cppdungeon::world::Map *map);
    void render(olc::PixelGameEngine *pge, olc::vf2d &offset);
    void followWithin(cppdungeon::entities::Entity *target, f32 distance) override;
};

#endif