#include "Game.hpp"

#ifdef WIN32
#include "wtypes.h"
#elif __APPLE__
#include <CoreGraphics/CGDisplayConfiguration.h>
#endif

bool cppdungeon::Game::OnUserCreate()
{
    tileRegistry = std::make_unique<cppdungeon::world::tiles::TileRegistry>();
    map = std::make_unique<cppdungeon::world::Map>(1000, 99, 99, tileRegistry.get());
    camera = std::make_unique<cppdungeon::gfx::Camera>(olc::vf2d{0, 0});
    entityManager = std::make_unique<cppdungeon::entities::EntityManager>();
    playerEntityID = entityManager->addEntity<cppdungeon::entities::Player>();
    entityManager->setPlayer(playerEntityID);
    player = dynamic_cast<cppdungeon::entities::Player *>(entityManager->getPlayer());
    player->setPosition(map->getSpawnPoint());

    hud = std::make_unique<cppdungeon::gfx::HUD>(player);

    u32 tinyId = entityManager->addEntity<cppdungeon::entities::TinyZombie>();
    entityManager->getEntity(tinyId)->setPosition(player->getPosition() + olc::vf2d{4, 4} + TILE_SIZE);
    entityManager->getEntity(tinyId)->followWithin(player, 1 * TILE_SIZE.x);
    return true;
}

bool cppdungeon::Game::OnUserUpdate(float fElapsedTime)
{
    // INPUT
    if (GetKey(olc::Key::ESCAPE).bHeld)
    {
        return false;
    }

    if (GetKey(olc::Key::E).bPressed)
    {
        player->getInventory()->toggleOpen();
    }

    if (!player->getInventory()->getIsOpen())
    {
        bool moveRight = GetKey(olc::Key::RIGHT).bHeld || GetKey(olc::Key::D).bHeld;
        bool moveLeft = GetKey(olc::Key::LEFT).bHeld || GetKey(olc::Key::A).bHeld;
        bool moveUp = GetKey(olc::Key::UP).bHeld || GetKey(olc::Key::W).bHeld;
        bool moveDown = GetKey(olc::Key::DOWN).bHeld || GetKey(olc::Key::S).bHeld;
        bool sprinting = GetKey(olc::Key::SHIFT).bHeld;
        i8 x = moveRight - moveLeft;
        i8 y = moveDown - moveUp;
        player->move(x, y, sprinting, fElapsedTime, map.get());
    }
    else
    {
        player->getInventory()->update(this, fElapsedTime);
    }

    if (GetKey(olc::Key::SPACE).bPressed)
    {
        map->regenerate(seed);
        player->setPosition(map->getSpawnPoint());
        seed++;
        level++;
    }

    if (GetKey(olc::Key::J).bPressed)
    {
        player->getInventory()->addItem(1, 1);
        player->getInventory()->addItem(2, 1);
        player->getInventory()->addItem(3, 1);
        player->getInventory()->addItem(4, 1);
        player->getInventory()->addItem(5, 1);
        player->getInventory()->addItem(6, 1);
        player->getInventory()->addItem(7, 1);
        player->getInventory()->addItem(8, 1);
        player->getInventory()->addItem(9, 1);
        player->getInventory()->addItem(10, 1);
        player->getInventory()->addItem(11, 1);
        player->getInventory()->addItem(12, 1);
    }
    else if (GetKey(olc::Key::K).bPressed)
    {
        player->getInventory()->removeItem(1, 1);
        player->getInventory()->removeItem(2, 1);
        player->getInventory()->removeItem(3, 1);
        player->getInventory()->removeItem(4, 1);
        player->getInventory()->removeItem(5, 1);
        player->getInventory()->removeItem(6, 1);
        player->getInventory()->removeItem(7, 1);
        player->getInventory()->removeItem(8, 1);
        player->getInventory()->removeItem(9, 1);
        player->getInventory()->removeItem(10, 1);
        player->getInventory()->removeItem(11, 1);
        player->getInventory()->removeItem(12, 1);
    }

    // UPDATE
    entityManager->updateAll(fElapsedTime, map.get());

    // RENDER
    camera->setOffset(player->getPosition() - GetScreenSize() / 2 + player->getSize() / 2);
    Clear(olc::Pixel(28, 17, 23));

    map->renderBackground(this, camera->getOffset(), GetScreenSize());
    entityManager->renderAll(this, camera->getOffset());
    map->renderForeground(this, camera->getOffset(), GetScreenSize());

    hud->render(this);
    player->renderInventory(this);

    DrawString({4, 16}, std::to_string(GetFPS()) + " FPS", olc::WHITE, 1);
    return true;
}

bool cppdungeon::Game::OnUserDestroy()
{
    return true;
}

int main()
{
    cppdungeon::u16 fullWidth = 0;
    cppdungeon::u16 fullHeight = 0;

#ifdef __APPLE__
    auto mainDisplayId = CGMainDisplayID();
    fullWidth = CGDisplayPixelsWide(mainDisplayId);
    fullHeight = CGDisplayPixelsHigh(mainDisplayId);
#elifdef WIN32
    fullWidth = GetSystemMetrics(SM_CXSCREEN);
    fullHeight = GetSystemMetrics(SM_CYSCREEN);
#else
    fullWidth = 1920;
    fullHeight = 1080;
#endif
    cppdungeon::i8 scale = 4;
    cppdungeon::Game demo;

    if (demo.Construct(fullWidth / scale, fullHeight / scale, scale, scale, true))
    {
        demo.Start();
    }

    return 0;
}