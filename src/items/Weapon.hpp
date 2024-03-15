#ifndef CPPDUNGEON_ITEMS_WEAPON_HPP
#define CPPDUNGEON_ITEMS_WEAPON_HPP

#include "Item.hpp"

namespace cppdungeon
{
    namespace items
    {
        class Weapon;
    }
}

class cppdungeon::items::Weapon : public cppdungeon::items::Item
{
private:
    u32 damage;
    u32 range;

public:
    Weapon(std::string itemName, std::string iconFile, u32 damage, u32 range): Item(itemName, iconFile, ItemType::WEAPON), damage(damage), range(range) {};
    u32 getDamage() { return damage; }
    u32 getRange() { return range; }
};

#endif