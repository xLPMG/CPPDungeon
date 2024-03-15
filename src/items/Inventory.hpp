#ifndef CPPDUNGEON_ITEMS_INVENTORY_HPP
#define CPPDUNGEON_ITEMS_INVENTORY_HPP

#include "Item.hpp"
#include <string>
#include <unordered_map>

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
    // ID, Quantity
    std::unordered_map<u32, u32> items;

    const cppdungeon::f32 inventoryScale = 0.8;
    std::unique_ptr<olc::Sprite> inventorySprite;
    std::unique_ptr<olc::Decal> inventoryDecal;

    bool hasCalculatedSizes = false;
    olc::vf2d invPosition = {0, 0};
    cppdungeon::f32 scale = 1;

public:
    Inventory();

    // Add item to inventory
    void addItem(u32 itemId, int amount);

    // Remove item from inventory
    void removeItem(u32 itemId, int amount);

    void update(f32 &deltaTime);
    void render(olc::PixelGameEngine *pge);
};

#endif