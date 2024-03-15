#ifndef CPPDUNGEON_ITEMS_INVENTORY_HPP
#define CPPDUNGEON_ITEMS_INVENTORY_HPP

#include "Item.hpp"
#include <string>
#include <vector>
#include "ItemManager.hpp"

namespace cppdungeon
{
    namespace items
    {
        class Inventory;
    }
}

class cppdungeon::items::Inventory
{
private:
    bool isOpen = false;

    // ID, Quantity
    std::vector<std::pair<u32,u32>> items;
    cppdungeon::items::ItemManager itemManager;

    const olc::vf2d inventoryScale = {0.35, 0.35};
    const olc::vf2d itemNameScale = {0.7, 0.7};
    std::unique_ptr<olc::Sprite> inventorySprite;
    std::unique_ptr<olc::Decal> inventoryDecal;

    bool hasCalculatedSizes = false;
    olc::vf2d invPosition = {0, 0};

    // inventory item visuals
    int selectedItem = 0;
    cppdungeon::f32 itemListCenterX = 0;
    cppdungeon::f32 itemListCenterY = 0;
    cppdungeon::i32 longNameStartPos = 0;

public:
    Inventory();

    void toggleOpen();

    bool getIsOpen() { return isOpen; }

    // Add item to inventory
    void addItem(u32 itemId, int amount);

    // Remove item from inventory
    void removeItem(u32 itemId, int amount);

    void update(olc::PixelGameEngine *pge, f32 &deltaTime);
    void render(olc::PixelGameEngine *pge);
};

#endif