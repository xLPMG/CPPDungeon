#include "Inventory.hpp"
#include <iostream>
#include "ItemManager.hpp"

cppdungeon::items::Inventory::Inventory()
{
    inventorySprite = std::make_unique<olc::Sprite>("./res/textures/ui/inventoryScreen.png");
    inventoryDecal = std::make_unique<olc::Decal>(inventorySprite.get());
}

void cppdungeon::items::Inventory::addItem(u32 itemId, int amount)
{
    if (items.find(itemId) != items.end())
    {
        items[itemId] += amount;
    }
    else
    {
        items[itemId] = amount;
    }
    if (items.at(itemId) <= 0)
    {
        items.erase(itemId);
    }
}

void cppdungeon::items::Inventory::removeItem(u32 itemId, int amount)
{
    addItem(itemId, -amount);
}

void cppdungeon::items::Inventory::render(olc::PixelGameEngine *pge)
{
    if (!hasCalculatedSizes)
    {
        scale = 0.8 * pge->ScreenWidth() / inventorySprite->width;
        invPosition = {pge->ScreenWidth() / 2 - inventorySprite->width * scale / 2, pge->ScreenHeight() / 2 - inventorySprite->height * scale / 2};
        hasCalculatedSizes = true;
    }

    pge->DrawDecal(invPosition, inventoryDecal.get(), {scale, scale});


    for (auto& it: items) {
    std::cout << itemManager.findItemById(it.first).getName() << " => " << it.second << std::endl;
}

}