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

public:
    // Add item to inventory
    void addItem(std::string itemName, u32 itemId, u16 itemQuantity, std::string iconFile);

    // Remove item from inventory
    void removeItem(u32 itemId, int amount);
};

#endif