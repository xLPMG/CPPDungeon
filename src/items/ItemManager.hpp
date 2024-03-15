#ifndef CPPDUNGEON_ITEMS_ITEM_MANAGER_HPP
#define CPPDUNGEON_ITEMS_ITEM_MANAGER_HPP

#include <unordered_map>
#include <string>
#include <stdexcept> // for std::out_of_range
#include "Item.hpp"
#include <memory>

namespace cppdungeon
{
    namespace items
    {
        class ItemManager;
    }
}

class cppdungeon::items::ItemManager
{
private:
    std::unordered_map<int, std::unique_ptr<cppdungeon::items::Item>> itemMap;
public:
    ItemManager();
    const Item *findItemById(int id);
};

#endif
