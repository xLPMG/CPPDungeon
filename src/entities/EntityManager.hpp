#ifndef CPPDUNGEON_ENTITIES_ENTITYMANAGER_HPP
#define CPPDUNGEON_ENTITIES_ENTITYMANAGER_HPP

#include <vector>
#include <memory>
#include "Entity.hpp"
#include "../constants.hpp"

namespace cppdungeon
{
    namespace entities
    {
        class EntityManager;
    }
}

class cppdungeon::entities::EntityManager
{
private:
    std::vector<std::unique_ptr<cppdungeon::entities::Entity>> entities;
    u32 playerIndex = 0;

public:
    template <typename T>
    u32 addEntity()
    {
        static_assert(std::is_base_of<Entity, T>::value, "T must be a derived class of Entity");
        entities.push_back(std::make_unique<T>());
        return entities.size() - 1;
    }
    cppdungeon::entities::Entity *getEntity(u32 index);
    void updateAll(f32 &deltaTime, cppdungeon::world::Map *map);
    void renderAll(olc::PixelGameEngine *pge, olc::vf2d offset);
    void clear();
    void setPlayer(u32 index);
    cppdungeon::entities::Entity *getPlayer();
};

#endif