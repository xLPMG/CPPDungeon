#include "ProjectileManager.hpp"
#include <iostream>

cppdungeon::projectiles::ProjectileManager::ProjectileManager()
{
}

void cppdungeon::projectiles::ProjectileManager::addProjectile(olc::vf2d position,
                                                               olc::vf2d direction,
                                                               cppdungeon::f32 speed,
                                                               olc::vf2d scale,
                                                               olc::Pixel tint,
                                                               i32 maxHits,
                                                               std::string spritePath)
{
    projectiles.push_back(std::make_unique<Projectile>(position,
                                                       direction,
                                                       speed,
                                                       scale,
                                                       tint,
                                                       maxHits,
                                                       spritePath));
}

void cppdungeon::projectiles::ProjectileManager::update(f32 &deltaTime,
                                                        cppdungeon::world::Map *map)
{
    for (auto &projectile : projectiles)
    {
        projectile->update(deltaTime, map);
    }

    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
                                     [](const std::unique_ptr<Projectile> &projectile)
                                     {
                                         return !projectile->getIsAlive();
                                     }),
                      projectiles.end());
}

void cppdungeon::projectiles::ProjectileManager::render(olc::PixelGameEngine *pge,
                                                        olc::vf2d offset)
{
    for (auto &projectile : projectiles)
    {
        projectile->render(pge, offset);
    }
}