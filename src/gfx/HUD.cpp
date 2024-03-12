#include "HUD.hpp"

cppdungeon::gfx::HUD::HUD(cppdungeon::entities::Entity *player)
{
    this->player = player;
    healthBarBG = std::make_unique<olc::Sprite>("./res/textures/ui/HUD/healthBarBG.png");
    healthBarFG = std::make_unique<olc::Sprite>("./res/textures/ui/HUD/healthBarFG.png");
}

void cppdungeon::gfx::HUD::render(olc::PixelGameEngine *pge)
{
    if(player == nullptr) return;

    pge->SetPixelMode(olc::Pixel::MASK);
    pge->DrawSprite({2, 2}, healthBarBG.get());

    i32 endX = player->getHealth() * healthBarFG.get()->width / player->getMaxHealth();
    pge->DrawPartialSprite({2, 2}, healthBarFG.get(), {0, 0}, {endX,healthBarBG.get()->height});
    pge->SetPixelMode(olc::Pixel::NORMAL);
}