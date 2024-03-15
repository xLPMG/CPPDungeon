#ifndef CPPDUNGEON_GAMEOBJECTS_ITEM_HPP
#define CPPDUNGEON_GAMEOBJECTS_ITEM_HPP

#include "../constants.hpp"
#include "olcPixelGameEngine.h"
#include <memory>
#include <iostream>
#include <string>

namespace cppdungeon
{
    namespace gameobjects
    {
        class Item;
    }
}

class cppdungeon::gameobjects::Item
{
private:
    std::string name;
    u32 id;
    u16 quantity;
    std::unique_ptr<olc::Sprite> sprite;
    std::unique_ptr<olc::Decal> decal;

public:
    Item(std::string itemName, u32 itemId, u16 itemQuantity, std::string iconFile);
    std::string getName() const;
    u32 getId() const;
    u16 getQuantity() const;
    void setQuantity(u16 newQuantity);
    void addQuantity(int amount);
    olc::Decal *getDecal() const;
};

#endif