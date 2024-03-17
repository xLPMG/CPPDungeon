#include "ItemManager.hpp"

cppdungeon::items::ItemManager::ItemManager()
{
    itemMap[1] = std::make_unique<Item>("Cool Sword", "./res/textures/items/MF_Items_weapon_1.png", ItemType::WEAPON);
    itemMap[3] = std::make_unique<Item>("Gold Sword", "./res/textures/items/MF_Items_weapon_3.png", ItemType::WEAPON);
    itemMap[2] = std::make_unique<Item>("Bronze Sword", "./res/textures/items/MF_Items_weapon_2.png", ItemType::WEAPON);
    itemMap[4] = std::make_unique<Item>("Looooong Sword", "./res/textures/items/MF_Items_weapon_4.png", ItemType::WEAPON);
    itemMap[5] = std::make_unique<Item>("whatisthis", "./res/textures/items/MF_Items_weapon_5.png");
    itemMap[6] = std::make_unique<Item>("really long debug item", "./res/textures/items/debug.png");

    itemMap[7] = std::make_unique<Item>("Tool 1", "./res/textures/items/MF_Items_tool_1.png");
    itemMap[8] = std::make_unique<Item>("Tool 2", "./res/textures/items/MF_Items_tool_2.png");
    itemMap[9] = std::make_unique<Item>("Tool 3", "./res/textures/items/MF_Items_tool_3.png");
    itemMap[10] = std::make_unique<Item>("Tool 4", "./res/textures/items/MF_Items_tool_4.png");
    itemMap[11] = std::make_unique<Item>("Tool 5", "./res/textures/items/MF_Items_tool_5.png");
    itemMap[12] = std::make_unique<Item>("Tool 6", "./res/textures/items/MF_Items_tool_6.png");
    itemMap[13] = std::make_unique<Item>("Tool 7", "./res/textures/items/MF_Items_tool_7.png");
    itemMap[14] = std::make_unique<Item>("Tool 8", "./res/textures/items/MF_Items_tool_8.png");
    itemMap[15] = std::make_unique<Item>("Tool 9", "./res/textures/items/MF_Items_tool_9.png");

    itemMap[16] = std::make_unique<Item>("Magic wand", "./res/textures/items/MF_Items_weapon_11.png", ItemType::WEAPON);
}

const cppdungeon::items::Item *cppdungeon::items::ItemManager::findItemById(int id)
{
    auto it = itemMap.find(id);
    if (it != itemMap.end())
    {
        return it->second.get();
    }
    else
    {
        std::cerr << "Item with id " << id << " not found" << std::endl;
    }
}