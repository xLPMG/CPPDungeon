#ifndef CPPDUNGEON_ITEMS_ITEMBAR_HPP
#define CPPDUNGEON_ITEMS_ITEMBAR_HPP

#include <vector>
#include "../items/Item.hpp"

namespace cppdungeon
{
    namespace items {
        class ItemBar;
    }
}

class cppdungeon::items::ItemBar
{
private:
    std::vector<std::unique_ptr<cppdungeon::items::Item>> items;
public:
    ItemBar();
    ~ItemBar();
    void render(olc::PixelGameEngine *pge);
    void update();
    void setItem(int index, int item);
    int getItem(int index);
    void setItemAmount(int index, int amount);
    int getItemAmount(int index);
};
#endif