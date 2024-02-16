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

        class Entity
        {
        protected:
            olc::vf2d position;
            olc::vf2d size;
            f32 speed = 50.0f;
            f32 sprintBoost = 2.0f;

        public:
            Entity(olc::vf2d position, olc::vf2d size) : position(position), size(size) {};
            virtual void update(f32 fElapsedTime) = 0;
            virtual void render(olc::PixelGameEngine *pge, olc::vf2d offset) = 0;
            virtual void move(i8 *x, i8 *y, bool sprinting, f32 *deltaTime, cppdungeon::world::Map* map) = 0;

            olc::vf2d getPosition()
            {
                return this->position;
            };

            olc::vf2d getSize()
            {
                return this->size;
            };
        };
    }
}

#endif