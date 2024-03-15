#include "Inventory.hpp"
#include <iostream>
#include "ItemManager.hpp"

cppdungeon::items::Inventory::Inventory()
{
    inventorySprite = std::make_unique<olc::Sprite>("./res/textures/ui/inventoryScreen.png");
    inventoryDecal = std::make_unique<olc::Decal>(inventorySprite.get());
}

void cppdungeon::items::Inventory::toggleOpen()
{
    isOpen = !isOpen;
}

void cppdungeon::items::Inventory::addItem(u32 itemId, int amount)
{
    for (auto it = items.begin(); it != items.end(); ++it)
    {
        if (it->first == itemId)
        {
            it->second += amount;
            if (it->second <= 0)
            {
                items.erase(it);
            }
            return;
        }
    }

    if (amount > 0)
        items.push_back(std::make_pair(itemId, amount));
}

void cppdungeon::items::Inventory::removeItem(u32 itemId, int amount)
{
    addItem(itemId, -amount);
}

void cppdungeon::items::Inventory::update(olc::PixelGameEngine *pge, f32 &deltaTime)
{
    if (isOpen)
    {
        if (pge->GetKey(olc::Key::UP).bPressed)
        {
            selectedItem--;
        }
        else if (pge->GetKey(olc::Key::DOWN).bPressed)
        {
            selectedItem++;
        }

        if (selectedItem < 0)
            selectedItem = 0;
        else if (selectedItem > items.size() - 1)
        {
            if (items.size() == 0)
            {
                selectedItem = 0;
            }
            else
            {
                selectedItem = items.size() - 1;
            }
        }
    }
}

void cppdungeon::items::Inventory::render(olc::PixelGameEngine *pge)
{
    if (!isOpen)
        return;

    if (!hasCalculatedSizes)
    {
        invPosition = {pge->ScreenWidth() / 2 - inventorySprite->width * inventoryScale.x / 2, pge->ScreenHeight() / 2 - inventorySprite->height * inventoryScale.y / 2};
        itemListCenterX = invPosition.x + inventorySprite->width * inventoryScale.x / 2;
        itemListCenterY = invPosition.y + inventorySprite->height * inventoryScale.y / 2 - 16.6f;
    }

    pge->DrawDecal(invPosition, inventoryDecal.get(), inventoryScale);

    ////////////////////////////////////////////////
    // DRAWING ITEMS
    ////////////////////////////////////////////////
    int size = items.size();
    for (int i = -5; i < 6; i++)
    {
        int index = selectedItem + i;
        if (index < 0 || index >= size)
        {
            continue;
        }

        const cppdungeon::items::Item *item = itemManager.findItemById(items[index].first);
        if (item == nullptr)
            return;

        ////////////////////////////////////////////////
        // DRAWING ITEM NAMES
        ////////////////////////////////////////////////
        f32 y = itemListCenterY + textHeight(itemNameScale.y) / 2 + i * 10.55f;
        std::string itemName = item->getName();

        if (itemName.length() > 16)
        {
            itemName = itemName.substr(longNameStartPos, 13)+"...";
        }
        if (i == 0)
        {
            pge->DrawStringDecal(olc::vf2d{itemListCenterX - textWidth(">" + itemName + ">", itemNameScale.x) / 2, y}, ">" + itemName + "<", olc::YELLOW, itemNameScale);
        }
        else
        {
            pge->DrawStringDecal(olc::vf2d{itemListCenterX - textWidth(itemName, itemNameScale.x) / 2, y}, itemName, olc::WHITE, itemNameScale);
        }

        ////////////////////////////////////////////////
        // DRAWING ITEM DATA
        ////////////////////////////////////////////////
        if (i == 0)
        {
            pge->DrawDecal(invPosition + olc::vf2d{198, 15}, item->getDecal());
            pge->DrawStringDecal(invPosition + olc::vf2d{206 - textWidth(std::to_string(items[index].second), 0.6) / 2, 41.2}, std::to_string(items[index].second), olc::WHITE, {0.6, 0.6});
            pge->DrawStringDecal(invPosition + olc::vf2d{182, 82}, "No stats available", olc::WHITE, {0.35, 0.35});
            pge->DrawStringDecal(invPosition + olc::vf2d{6, 140}, item->getName(), olc::WHITE, {0.7, 0.7});
        }
    }
}