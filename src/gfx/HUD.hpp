#ifndef CPPDUNGEON_GFX_HUD_HPP
#define CPPDUNGEON_GFX_HUD_HPP

#define OLC_IGNORE_VEC2D
#include "olcUTIL_Geometry2D.h"
#include "olcPixelGameEngine.h"

#include "../entities/Entity.hpp"
#include <memory>

namespace cppdungeon
{
    namespace gfx
    {
        class HUD;
    }
}

class cppdungeon::gfx::HUD
{
private:
    cppdungeon::entities::Entity *player = nullptr;
    std::unique_ptr<olc::Sprite> healthBarBG;
    std::unique_ptr<olc::Sprite> healthBarFG;

public:
    HUD(cppdungeon::entities::Entity *player);
    void render(olc::PixelGameEngine *pge);
};

#endif