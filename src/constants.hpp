/**
 * Constants / typedefs used throughout the code.
 *
 * @author Luca-Philipp Grumbach
 **/
#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>
#include <cstddef>
#include <sys/types.h>

#define OLC_IGNORE_VEC2D
#include <olcUTIL_Geometry2D.h>

namespace cppdungeon
{
    typedef float f32;
    typedef double f64;
    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;
    typedef int8_t i8;
    typedef int16_t i16;
    typedef int32_t i32;
    typedef int64_t i64;
    typedef size_t usize;
    typedef ssize_t isize;

#define PI 3.14159265359f
#define TAU (2.0f * PI)
#define PI_2 (PI / 2.0f)
#define PI_4 (PI / 4.0f)

#define DEG2RAD(_d) ((_d) * (PI / 180.0f))
#define RAD2DEG(_d) ((_d) * (180.0f / PI))

    typedef struct v2_s
    {
        f32 x, y;
    } v2;
    typedef struct v2i_s
    {
        i32 x, y;
    } v2i;

    typedef struct rect_s
    {
        f32 x, y, width, height;
    } rect;

    typedef struct recti_s
    {
        i32 x, y, width, height;
    } recti;

    typedef struct rectu_s
    {
        u32 x, y, width, height;
    } rectu;

    enum class Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    enum class ItemType
    {
        WEAPON,
        ARMOR,
        CONSUMABLE,
        MISC
    };

    static inline i32 idx(i32 x, i32 y, i32 width)
    {
        return y * width + x;
    }

    static inline i32 idx(olc::vi2d pos, i32 width)
    {
        return pos.y * width + pos.x;
    }

    static constexpr olc::vi2d TILE_SIZE = { 16,16 };

    static inline f32 textWidth(const std::string &text, f32 scale)
    {
        return text.length() * 8 * scale;
    }

        static inline f32 textHeight(f32 scale)
    {
        return 8 * scale;
    }

}

#endif
