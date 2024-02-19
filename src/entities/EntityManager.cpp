#include "EntityManager.hpp"

cppdungeon::entities::Entity *cppdungeon::entities::EntityManager::getEntity(u32 index)
{
    return entities.at(index).get();
}

void cppdungeon::entities::EntityManager::updateAll(f32 &deltaTime)
{
    for (auto &entity : entities)
    {
        entity->update(deltaTime);
    }
}

void cppdungeon::entities::EntityManager::renderAll(olc::PixelGameEngine *pge, olc::vf2d offset)
{
    for (auto &entity : entities)
    {
        entity->render(pge, offset);
    }
}

void cppdungeon::entities::EntityManager::clear()
{
    entities.clear();
}