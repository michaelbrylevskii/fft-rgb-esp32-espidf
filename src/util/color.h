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

typedef union
{
    int16_t bytes[3];
    struct
    {
        int16_t blue;
        int16_t green;
        int16_t red;
    };
    struct
    {
        int16_t b;
        int16_t g;
        int16_t r;
    };
} rgbx_t;

typedef union
{
    int16_t bytes[4];
    struct
    {
        int16_t blue;
        int16_t green;
        int16_t red;
        int16_t alpha;
    };
    struct
    {
        int16_t b;
        int16_t g;
        int16_t r;
        int16_t a;
    };
} rgbax_t;

int8_t cmp_rgb(rgb_t a, rgb_t b);

int8_t cmp_rgba(rgba_t a, rgba_t b);

rgbx_t sub_rgb(rgb_t a, rgb_t b);

rgbax_t sub_rgba(rgba_t a, rgba_t b);

rgb_t rnd_rgb();

rgb_t rnd_colorful_rgb();
