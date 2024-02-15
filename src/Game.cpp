#include "Game.hpp"
#include "entities/Player.hpp"

#if WIN32
#include <windows.h>
#else
#include <CoreGraphics/CGDisplayConfiguration.h>
#endif

bool cppdungeon::Game::OnUserCreate()
{
    tileRegistry = new cppdungeon::world::tiles::TileRegistry();
    map = new world::Map(1000, 100, 100, olc::vf2d{16, 16}, tileRegistry);
    camera = new cppdungeon::gfx::Camera({0, 0});
    player = new cppdungeon::entities::Player();
    entities.push_back(player);

    return true;
}

bool cppdungeon::Game::OnUserUpdate(float fElapsedTime)
{
    // INPUT
    bool moveRight = GetKey(olc::Key::RIGHT).bHeld || GetKey(olc::Key::D).bHeld;
    bool moveLeft = GetKey(olc::Key::LEFT).bHeld || GetKey(olc::Key::A).bHeld;
    bool moveUp = GetKey(olc::Key::UP).bHeld || GetKey(olc::Key::W).bHeld;
    bool moveDown = GetKey(olc::Key::DOWN).bHeld || GetKey(olc::Key::S).bHeld;
    bool sprinting = GetKey(olc::Key::SHIFT).bHeld;
    i8 x = moveRight - moveLeft;
    i8 y = moveDown - moveUp;
    player->move(&x, &y, sprinting, &fElapsedTime, map);

    // UPDATE
    for (auto &entity : entities)
    {
        entity->update(fElapsedTime);
    }

    // RENDER
    camera->setOffset(player->getPosition() - GetScreenSize() / 2 + player->getSize() / 2);
    Clear(olc::Pixel(28, 17, 23));

    map->renderBackground(this, camera->getOffset(), GetScreenSize());
    for (auto &entity : entities)
    {
        entity->render(this, camera->getOffset());
    }
    return true;
}

bool cppdungeon::Game::OnUserDestroy()
{
    delete tileRegistry;
    delete map;
    delete camera;
    for (auto &entity : entities)
    {
        delete entity;
    }
    return true;
}

int main()
{
    cppdungeon::u32 width = 0;
    cppdungeon::u32 height = 0;
#if WIN32
    width = (int)GetSystemMetrics(SM_CXSCREEN);
    height = (int)GetSystemMetrics(SM_CYSCREEN);
#else
    auto mainDisplayId = CGMainDisplayID();
    width = CGDisplayPixelsWide(mainDisplayId);
    height = CGDisplayPixelsHigh(mainDisplayId);
#endif

    cppdungeon::Game demo;
    if (demo.Construct(width / 4, height / 4, 4, 4))
        demo.Start();

    return 0;
}