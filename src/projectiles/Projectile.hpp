#ifndef CPPDUNGEON_ENTITIES_PROJECTILE_HPP
#define CPPDUNGEON_ENTITIES_PROJECTILE_HPP

#include "../constants.hpp"
#include "../world/Map.hpp"
#define OLC_IGNORE_VEC2D
#include "olcUTIL_Geometry2D.h"
#include "olcPixelGameEngine.h"

namespace cppdungeon
{
    namespace projectiles
    {
        class Projectile;
    }
}

class cppdungeon::projectiles::Projectile
{
private:
    olc::vf2d position;
    olc::vf2d direction;
    cppdungeon::f32 speed;
    olc::vf2d scale;
    olc::Pixel tint;
    cppdungeon::i32 hits = 0;
    cppdungeon::i32 maxHits = 3;
    cppdungeon::f32 damage = 1.0f;
    bool isAlive = true;
    
    std::unique_ptr<olc::Sprite> sprite;
    std::unique_ptr<olc::Decal> decal;

    bool handleCollision(const olc::vf2d &point, const olc::vf2d &potentialPosition, const olc::vf2d &radialDims, cppdungeon::world::Map *map);

public:
    Projectile(olc::vf2d position, olc::vf2d direction, cppdungeon::f32 speed, olc::vf2d scale, olc::Pixel tint, cppdungeon::i32 maxHits, std::string spritePath);
    void update(f32 &deltaTime, cppdungeon::world::Map *map);
    void render(olc::PixelGameEngine *pge, olc::vf2d &offset);
    bool getIsAlive();
};

#endif