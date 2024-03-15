#ifndef CPPDUNGEON_ITEMS_ITEM_MANAGER_HPP
#define CPPDUNGEON_ITEMS_ITEM_MANAGER_HPP

#include <unordered_map>
#include <string>
#include <stdexcept> // for std::out_of_range
#include "Item.hpp"

namespace cppdungeon
{
    namespace items
    {
        class ItemManager;
    }
}

class cppdungeon::items::ItemManager {
private:
    static const std::unordered_map<int, Item> itemMap = {
        {1, Item("Health Potion", "icon")}
    };
public:
    static const std::unordered_map<int, Item>& getItemMap() {
        return itemMap;
    }

    static const Item& findItemById(int id) {
        auto it = itemMap.find(id);
        if (it != itemMap.end()) {
            return it->second;
        } else {
            throw std::out_of_range("Item not found");
        }
    }
};

#endif
