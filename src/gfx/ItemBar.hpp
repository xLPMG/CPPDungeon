#ifndef CPPDUNGEON_GFX_ITEMBAR_HPP
#define CPPDUNGEON_GFX_ITEMBAR_HPP

#include <vector>
#include "../gameobjects/Item.hpp"

namespace cppdungeon
{
    namespace gfx {
        class ItemBar;
    }
}

class cppdungeon::gfx::ItemBar
{
private:
    std::vector<std::unique_ptr<cppdungeon::gameobjects::Item>> items;
public:
    ItemBar();
    ~ItemBar();
    void draw();
    void update();
    void setItem(int index, int item);
    int getItem(int index);
    void setItemAmount(int index, int amount);
    int getItemAmount(int index);
};
#endif