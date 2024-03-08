#ifndef CPPDUNGEON_GAMEOBJECTS_ITEM_HPP
#define CPPDUNGEON_GAMEOBJECTS_ITEM_HPP

#include "../constants.hpp"
#include "olcPixelGameEngine.h"
#include <memory>
#include <iostream>
#include <string>

namespace cppdungeon
{
    namespace gameobjects {
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

public:
    Item(std::string itemName, u32 itemId, u16 itemQuantity, std::string iconFile) : id(itemId), quantity(itemQuantity){
        setName(itemName);
        this->sprite = std::make_unique<olc::Sprite>(iconFile);
    }

    std::string getName() const {
        return name;
    }

    void setName(const std::string& itemName) {
        if (name.empty()) {
            name = itemName;
        } else {
            if (name != itemName) {
                std::cerr << "Error: Attempted to bind different names to the same item ID." << std::endl;
            }
        }
    }

    u32 getId() const {
        return id;
    }

    u16 getQuantity() const {
        return quantity;
    }

    void setQuantity(u16 newQuantity) {
        quantity = newQuantity;
    }

    void addQuantity(int amount) {
        quantity = std::max(0, quantity + amount);
    }

    olc::Sprite *getSprite() const {
        return sprite.get();
    }
};

#endif