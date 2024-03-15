#ifndef CPPDUNGEON_ITEMS_ITEM_HPP
#define CPPDUNGEON_ITEMS_ITEM_HPP

#include "../constants.hpp"
#include "olcPixelGameEngine.h"
#include <memory>
#include <iostream>
#include <string>

namespace cppdungeon
{
    namespace items
    {
        class Item;
    }
}

class cppdungeon::items::Item
{
private:
    std::string name;
    ItemType type;
    std::unique_ptr<olc::Sprite> sprite;
    std::unique_ptr<olc::Decal> decal;

public:
    Item(std::string itemName, std::string iconFile, ItemType itemType = ItemType::MISC);
    std::string getName() const;
    ItemType getType() const;
    olc::Decal *getDecal() const;
};

#endif