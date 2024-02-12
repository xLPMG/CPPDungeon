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

#define ASSERT(_e, ...)               \
    if (!(_e))                        \
    {                                 \
        fprintf(stderr, __VA_ARGS__); \
        exit(1);                      \
    }

#define PI 3.14159265359f
#define TAU (2.0f * PI)
#define PI_2 (PI / 2.0f)
#define PI_4 (PI / 4.0f)

#define DEG2RAD(_d) ((_d) * (PI / 180.0f))
#define RAD2DEG(_d) ((_d) * (180.0f / PI))

#define SCREEN_WIDTH 384
#define SCREEN_HEIGHT 216

    typedef struct v2_s
    {
        f32 x, y;
    } v2;
    typedef struct v2i_s
    {
        i32 x, y;
    } v2i;

    enum class Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

}

#endif