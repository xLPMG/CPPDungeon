#ifndef CPPDUNGEON_WORLD_GENERATOR_HPP
#define CPPDUNGEON_WORLD_GENERATOR_HPP

#include "../constants.hpp"
#include <vector>

namespace cppdungeon
{
    namespace world
    {
        class Generator;
    }
}

class cppdungeon::world::Generator
{

public:
    static void generate(i32 seed, i32 width, i32 height, std::vector<u16> &tiles);
};

#endif