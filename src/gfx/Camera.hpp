#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "olcPixelGameEngine.h"
#include "../constants.hpp"

namespace cppdungeon
{
    namespace gfx
    {
        class Camera;
    }
}

class cppdungeon::gfx::Camera
{

private:
    olc::vf2d position;

public:
    Camera(olc::vf2d position){
        this->position = position;
    };

    olc::vf2d getOffset(){
        return this->position;
    };

    void move(f32 x, f32 y){
        this->position.x += x;
        this->position.y += y;
    };

    void setOffset(olc::vf2d position){
        this->position = position;
    };
};

#endif