#include "Animation.hpp"
#include <iostream>

cppdungeon::gfx::Animation::Animation(std::string file, i8 frameWidth, i8 frameHeight, f32 frameTime, bool repeating) : frameWidth(frameWidth), frameHeight(frameHeight), frameTime(frameTime), repeating(repeating)
{
    this->sprite = std::make_unique<olc::Sprite>(file);
    frames = sprite->width / frameWidth;
    lastUpdate = std::chrono::high_resolution_clock::now();
}

void cppdungeon::gfx::Animation::update(float fElapsedTime)
{
    if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - lastUpdate).count() > 200){
        currentTime = 0;
        currentFrame = 0;
    }
    lastUpdate = std::chrono::high_resolution_clock::now();

    if(!repeating && currentFrame >= frames-1){
        return;
    }

    currentTime += fElapsedTime;
    if (currentTime >= frameTime)
    {
        currentTime = 0;
        currentFrame++;
        if (repeating && currentFrame >= frames)
        {
            currentFrame = 0;
        }
    }
}

void cppdungeon::gfx::Animation::render(olc::PixelGameEngine *pge, olc::vi2d position)
{
    pge->SetPixelMode(olc::Pixel::MASK);
    pge->DrawPartialSprite(position, sprite.get(), {currentFrame * frameWidth, 0}, {frameWidth, frameHeight}, 1, flipped);
    pge->SetPixelMode(olc::Pixel::NORMAL);
}

void cppdungeon::gfx::Animation::flip(u8 flipped)
{
    this->flipped = flipped;
}