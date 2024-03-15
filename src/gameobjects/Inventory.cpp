#include "Inventory.hpp"

cppdungeon::gameobjects::Inventory::Inventory()
{
    inventorySprite = std::make_unique<olc::Sprite>("./res/textures/ui/inventoryScreen.png");
    inventoryDecal = std::make_unique<olc::Decal>(inventorySprite.get());
}

void cppdungeon::gameobjects::Inventory::addItem(std::string itemName, u32 itemId, u16 itemQuantity, std::string iconFile)
{
    if (items.find(itemId) == items.end())
    {
        items.insert(std::make_pair(itemId, std::make_unique<Item>(itemName, itemId, itemQuantity, iconFile)));
    }
    else
    {
        items.at(itemId)->addQuantity(itemQuantity);
    }
}

void cppdungeon::gameobjects::Inventory::removeItem(u32 itemId, int amount)
{
    if (items.find(itemId) != items.end())
    {
        items.at(itemId)->addQuantity(amount);
        if (items.at(itemId)->getQuantity() == 0)
        {
            items.erase(itemId);
        }
    }
}

void cppdungeon::gameobjects::Inventory::render(olc::PixelGameEngine *pge)
{
    if (!hasCalculatedSizes)
    {
        scale = 0.8 * pge->ScreenWidth() / inventorySprite->width;
        invPosition = {pge->ScreenWidth() / 2 - inventorySprite->width * scale / 2, pge->ScreenHeight() / 2 - inventorySprite->height * scale / 2};
        hasCalculatedSizes = true;
    }

    pge->DrawDecal(invPosition, inventoryDecal.get(), {scale, scale});
}