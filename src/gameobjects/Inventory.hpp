#ifndef CPPDUNGEON_GAMEOBJECTS_INVENTORY_HPP
#define CPPDUNGEON_GAMEOBJECTS_INVENTORY_HPP

#include "Item.hpp"
#include <string>
#include <unordered_map>

namespace cppdungeon
{
    namespace gameobjects
    {
        class Inventory;
    }
}

class cppdungeon::gameobjects::Inventory
{
private:
    std::unordered_map<u32, std::unique_ptr<Item>> items;

    const cppdungeon::f32 inventoryScale = 0.8;
    std::unique_ptr<olc::Sprite> inventorySprite;
    std::unique_ptr<olc::Decal> inventoryDecal;

    bool hasCalculatedSizes = false;
    olc::vf2d invPosition = {0, 0};
    cppdungeon::f32 scale = 1;

public:
    Inventory();

    // Add item to inventory
    void addItem(std::string itemName, u32 itemId, u16 itemQuantity, std::string iconFile);

    // Remove item from inventory
    void removeItem(u32 itemId, int amount);

    void update(f32 &deltaTime);
    void render(olc::PixelGameEngine *pge);
};

#endif