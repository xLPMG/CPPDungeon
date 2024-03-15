#include "Item.hpp"

cppdungeon::gameobjects::Item::Item(std::string itemName, u32 itemId, u16 itemQuantity, std::string iconFile)
{
    name = itemName;
    id = itemId;
    quantity = itemQuantity;
    sprite = std::make_unique<olc::Sprite>(iconFile);
    decal = std::make_unique<olc::Decal>(sprite.get());
}

std::string cppdungeon::gameobjects::Item::getName() const
{
    return name;
}

cppdungeon::u32 cppdungeon::gameobjects::Item::getId() const
{
    return id;
}

cppdungeon::u16 cppdungeon::gameobjects::Item::getQuantity() const
{
    return quantity;
}

void cppdungeon::gameobjects::Item::setQuantity(u16 newQuantity)
{
    quantity = newQuantity;
}

void cppdungeon::gameobjects::Item::addQuantity(int amount)
{
    quantity = std::max(0, quantity + amount);
}

olc::Decal *cppdungeon::gameobjects::Item::getDecal() const
{
    return decal.get();
}