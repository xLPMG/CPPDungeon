#include "EntityManager.hpp"

cppdungeon::entities::Entity *cppdungeon::entities::EntityManager::getEntity(u32 index)
{
    return entities.at(index).get();
}

void cppdungeon::entities::EntityManager::updateAll(f32 &deltaTime, cppdungeon::world::Map *map)
{
    for (auto &entity : entities)
    {
        entity->update(deltaTime, map);
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

void cppdungeon::entities::EntityManager::setPlayer(u32 index)
{
    playerIndex = index;
}
cppdungeon::entities::Entity *cppdungeon::entities::EntityManager::getPlayer()
{
    return entities.at(playerIndex).get();
}