#ifndef CPPDUNGEON_ENTITIES_ENTITY_HPP
#define CPPDUNGEON_ENTITIES_ENTITY_HPP

#include "../constants.hpp"
#define OLC_IGNORE_VEC2D
#include "olcUTIL_Geometry2D.h"
#include "olcPixelGameEngine.h"
#include "../gfx/Camera.hpp"
#include "../world/Map.hpp"

namespace cppdungeon
{
    namespace entities
    {
        class Entity;
    }
}

class cppdungeon::entities::Entity
{
protected:
    olc::vf2d position;
    olc::vf2d size;
    f32 speed = 50.0f;
    f32 sprintBoost = 2.0f;
    cppdungeon::entities::Entity *followTarget = nullptr;
    f32 followRange = 0.0f;

    enum class Direction
    {
        LEFT,
        RIGHT,
        UP,
        DOWN,
        IDLE
    };

public:
    Entity(olc::vf2d position, olc::vf2d size) : position(position), size(size){};
    virtual void update(f32 &deltaTime, cppdungeon::world::Map *map) = 0;
    virtual void render(olc::PixelGameEngine *pge, olc::vf2d &offset) = 0;

    void setPosition(olc::vf2d position)
    {
        this->position = position;
    };

    olc::vf2d getPosition()
    {
        return this->position;
    };

    olc::vf2d getSize()
    {
        return this->size;
    };

    virtual void followWithin(cppdungeon::entities::Entity *target, f32 distance){}

    void attackWithin(cppdungeon::entities::Entity *target, f32 distance){}    
};

#endif