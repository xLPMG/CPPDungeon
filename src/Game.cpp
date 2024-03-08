#include "Game.hpp"

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
    entityManager = std::make_unique<cppdungeon::entities::EntityManager>();
    playerEntityID = entityManager->addEntity<cppdungeon::entities::Player>();
    entityManager->setPlayer(playerEntityID);
    player = dynamic_cast<cppdungeon::entities::Player*>(entityManager->getPlayer());
    player->setPosition(map->getSpawnPoint());

    hud = std::make_unique<cppdungeon::gfx::HUD>(player);

    u32 tinyId = entityManager->addEntity<cppdungeon::entities::TinyZombie>();
    entityManager->getEntity(tinyId)->setPosition(player->getPosition() + olc::vf2d{4, 4}+TILE_SIZE);
    entityManager->getEntity(tinyId)->followWithin(player, 1*TILE_SIZE.x);
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
        level++;
    }

    // UPDATE
    entityManager->updateAll(fElapsedTime, map.get());

    // RENDER
    camera->setOffset(player->getPosition() - GetScreenSize() / 2 + player->getSize() / 2);
    Clear(olc::Pixel(28, 17, 23));

    map->renderBackground(this, camera->getOffset(), GetScreenSize());
    entityManager->renderAll(this, camera->getOffset());
    map->renderForeground(this, camera->getOffset(), GetScreenSize());

    DrawString({4, 4}, std::to_string(level), olc::WHITE, 1);
    hud->render(this);
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
    cppdungeon::i8 scale = 4;
    cppdungeon::Game demo;
    if (demo.Construct(width / scale, height / scale, scale, scale))
        demo.Start();

    return 0;
}