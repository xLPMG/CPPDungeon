#include "Inventory.hpp"

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