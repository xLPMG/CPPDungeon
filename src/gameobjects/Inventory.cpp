#include "Inventory.hpp"

void cppdungeon::gameobjects::Inventory::addItem(const Item &item)
{
    int itemId = item.getId();
    if (items.find(itemId) == items.end())
    {
        items.insert(std::make_pair(itemId, item));
    }
    else
    {
        items[itemId].addQuantity(item.getQuantity());
    }
}

void cppdungeon::gameobjects::Inventory::removeItem(u32 itemId, int amount)
{
    if (items.find(itemId) != items.end())
    {
        items[itemId].addQuantity(amount);
        if (items[itemId].getQuantity() == 0)
        {
            items.erase(itemId);
        }
    }
}