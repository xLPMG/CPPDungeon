#include "Item.hpp"

cppdungeon::items::Item::Item(std::string itemName, std::string iconFile)
{
    name = itemName;
    sprite = std::make_unique<olc::Sprite>(iconFile);
    decal = std::make_unique<olc::Decal>(sprite.get());
}

std::string cppdungeon::items::Item::getName() const
{
    return name;
}

cppdungeon::u32 cppdungeon::items::Item::getId() const
{
    return id;
}

cppdungeon::u16 cppdungeon::items::Item::getQuantity() const
{
    return quantity;
}

void cppdungeon::items::Item::setQuantity(u16 newQuantity)
{
    quantity = newQuantity;
}

void cppdungeon::items::Item::addQuantity(int amount)
{
    quantity = std::max(0, quantity + amount);
}

olc::Decal *cppdungeon::items::Item::getDecal() const
{
    return decal.get();
}