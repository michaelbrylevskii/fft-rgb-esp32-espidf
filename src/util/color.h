#pragma once

#include <stdint.h>

typedef union
{
    uint8_t bytes[3];
    struct
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
    };
    struct
    {
        uint8_t b;
        uint8_t g;
        uint8_t r;
    };
} rgb_t;

typedef union
{
    uint8_t bytes[4];
    struct
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha;
    };
    struct
    {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t a;
    };
} rgba_t;
