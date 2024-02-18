#include "Game.hpp"
#include "entities/Player.hpp"
#include "world/Map.hpp"

#include <memory>

#ifdef WIN32
#include <windows.h>
#elif __APPLE__
#include <CoreGraphics/CGDisplayConfiguration.h>
#endif

bool cppdungeon::Game::OnUserCreate()
{
    tileRegistry = std::make_unique<cppdungeon::world::tiles::TileRegistry>();
    map = std::make_unique<cppdungeon::world::Map>(1000, 99, 99, tileRegistry.get());
    camera = std::make_unique<cppdungeon::gfx::Camera>(olc::vf2d{0,0});
    player = std::make_unique<cppdungeon::entities::Player>(olc::vf2d{0, 0});
    player->setPosition(map->getSpawnPoint());
    return true;
}

bool cppdungeon::Game::OnUserUpdate(float fElapsedTime)
{
    // INPUT
    if(GetKey(olc::Key::ESCAPE).bHeld){
        return false;
    }
    bool moveRight = GetKey(olc::Key::RIGHT).bHeld || GetKey(olc::Key::D).bHeld;
    bool moveLeft = GetKey(olc::Key::LEFT).bHeld || GetKey(olc::Key::A).bHeld;
    bool moveUp = GetKey(olc::Key::UP).bHeld || GetKey(olc::Key::W).bHeld;
    bool moveDown = GetKey(olc::Key::DOWN).bHeld || GetKey(olc::Key::S).bHeld;
    bool sprinting = GetKey(olc::Key::SHIFT).bHeld;
    i8 x = moveRight - moveLeft;
    i8 y = moveDown - moveUp;
    player->move(x, y, sprinting, fElapsedTime, map.get());

    if(GetKey(olc::Key::SPACE).bPressed){
        map->regenerate(seed);
        player->setPosition(map->getSpawnPoint());
        seed++;
    }

    // UPDATE
    player->update(fElapsedTime);

    // RENDER
    camera->setOffset(player->getPosition() - GetScreenSize() / 2 + player->getSize() / 2);
    Clear(olc::Pixel(28, 17, 23));

    map->renderBackground(this, camera->getOffset(), GetScreenSize());
    player->render(this, camera->getOffset());
    map->renderForeground(this, camera->getOffset(), GetScreenSize());

    return true;
}

bool cppdungeon::Game::OnUserDestroy()
{
    return true;
}

int main()
{
    cppdungeon::u32 width = 0;
    cppdungeon::u32 height = 0;
#ifdef __APPLE__
    auto mainDisplayId = CGMainDisplayID();
    width = CGDisplayPixelsWide(mainDisplayId);
    height = CGDisplayPixelsHigh(mainDisplayId);
#else
    width = 1920;
    height = 1080;
#endif
    cppdungeon::i8 scale = 1;
    cppdungeon::Game demo;
    if (demo.Construct(width / scale, height / scale, scale, scale))
        demo.Start();

    return 0;
}