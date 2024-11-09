#include "color.h"

#include "esp_random.h"

int8_t cmp_rgb(rgb_t a, rgb_t b)
{
    if (a.r > b.r)
        return 1;
    if (a.r < b.r)
        return -1;
    if (a.g > b.g)
        return 1;
    if (a.g < b.g)
        return -1;
    if (a.b > b.b)
        return 1;
    if (a.b < b.b)
        return -1;
    return 0;
}

int8_t cmp_rgba(rgba_t a, rgba_t b)
{
    if (a.r > b.r)
        return 1;
    if (a.r < b.r)
        return -1;
    if (a.g > b.g)
        return 1;
    if (a.g < b.g)
        return -1;
    if (a.b > b.b)
        return 1;
    if (a.b < b.b)
        return -1;
    if (a.a > b.a)
        return 1;
    if (a.a < b.a)
        return -1;
    return 0;
}

rgbx_t sub_rgb(rgb_t a, rgb_t b)
{
    return (rgbx_t){
        .r = a.r - b.r,
        .g = a.g - b.g,
        .b = a.b - b.b,
    };
}

rgbax_t sub_rgba(rgba_t a, rgba_t b)
{
    return (rgbax_t){
        .r = a.r - b.r,
        .g = a.g - b.g,
        .b = a.b - b.b,
        .a = a.a - b.a,
    };
}

rgb_t rnd_rgb()
{
    rgb_t color = {0};
    esp_fill_random(&color, sizeof(color));
    return color;
}

rgb_t rnd_colorful_rgb()
{
    rgb_t color = {0};
    uint32_t rnd = esp_random();
    uint8_t value = rnd & 0xff;
    uint8_t variant = ((rnd >> 8) & 0xffffff) % 6;

    if (variant == 0)
    {
        color.r = value;
        color.g = 255;
    }
    else if (variant == 1)
    {
        color.r = value;
        color.b = 255;
    }
    else if (variant == 2)
    {
        color.g = value;
        color.r = 255;
    }
    else if (variant == 3)
    {
        color.g = value;
        color.b = 255;
    }
    else if (variant == 4)
    {
        color.b = value;
        color.r = 255;
    }
    else
    {
        color.b = value;
        color.g = 255;
    }
    return color;
}
