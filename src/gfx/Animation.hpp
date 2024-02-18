#ifndef CPPDUNGEON_GFX_ANIMATION_HPP
#define CPPDUNGEON_GFX_ANIMATION_HPP

#define OLC_IGNORE_VEC2D
#include "olcUTIL_Geometry2D.h"
#include "olcPixelGameEngine.h"
#include "../constants.hpp"
#include <chrono>

namespace cppdungeon
{
    namespace gfx
    {
        class Animation;
    }
}

class cppdungeon::gfx::Animation
{
private:
    std::unique_ptr<olc::Sprite> sprite;
    i8 frameWidth;
    i8 frameHeight;
    f32 frameTime;
    bool repeating;
    i8 frames;

    i8 currentFrame;
    f32 currentTime;

    u8 flipped = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastUpdate;

public:
    Animation(std::string file, i8 frameWidth, i8 frameHeight, f32 frameTime, bool repeating = true);
    void update(float fElapsedTime);
    void render(olc::PixelGameEngine *pge, olc::vi2d position);
    void flip(u8 flip);
};

#endif