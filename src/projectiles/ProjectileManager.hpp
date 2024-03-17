#ifndef CPPDUNGEON_PROJECTILES_PROJECTILEMANAGER_HPP
#define CPPDUNGEON_PROJECTILES_PROJECTILEMANAGER_HPP

#include "Projectile.hpp"
#include "../constants.hpp"
#include "../world/Map.hpp"
#define OLC_IGNORE_VEC2D
#include <olcUTIL_Geometry2D.h>
#include <olcPixelGameEngine.h>

#include <vector>
#include <memory>

namespace cppdungeon
{
    namespace projectiles
    {
        class ProjectileManager;
    }
}

class cppdungeon::projectiles::ProjectileManager
{
private:
    std::vector<std::unique_ptr<Projectile>> projectiles;

public:
    ProjectileManager();
    void addProjectile(olc::vf2d position,
                       olc::vf2d direction,
                       cppdungeon::f32 speed,
                       olc::vf2d scale,
                       olc::Pixel tint,
                       i32 maxHits,
                       std::string spritePath);
    void update(f32 &deltaTime,
                cppdungeon::world::Map *map);
    void render(olc::PixelGameEngine *pge,
                olc::vf2d offset);
};

#endif
