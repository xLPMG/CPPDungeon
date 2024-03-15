#include "Item.hpp"

cppdungeon::items::Item::Item(std::string itemName, std::string iconFile, ItemType itemType)
{
    name = itemName;
    type = itemType;
    sprite = std::make_unique<olc::Sprite>(iconFile);
    decal = std::make_unique<olc::Decal>(sprite.get());
}

std::string cppdungeon::items::Item::getName() const
{
    return name;
}

cppdungeon::ItemType cppdungeon::items::Item::getType() const
{
    return type;
}

olc::Decal *cppdungeon::items::Item::getDecal() const
{
    return decal.get();
}