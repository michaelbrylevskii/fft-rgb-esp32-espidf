#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_random.h"

#include "config.h"
#include "util/error.h"
#include "util/color.h"
#include "util/hw/rgb_ledc.h"

void delay_ms(uint16_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void init_rgb_led()
{
    rgb_ledc_config_t rgb_ledc = {
        .led = RGB_LED_0,
        .timer = LEDC_TIMER_0,
        .channel_r = LEDC_CHANNEL_0,
        .channel_g = LEDC_CHANNEL_1,
        .channel_b = LEDC_CHANNEL_2,
        .gpio_r = LED_R_GPIO,
        .gpio_g = LED_G_GPIO,
        .gpio_b = LED_B_GPIO,
        .freq_hz = 2000,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_12_BIT,
        // .duty_references = {0, 255, 512, 1023, 0, 255, 512, 1023}, // 10 bit test
        // .duty_references = {0, 20, 82, 167, 331, 521, 748, 1023},  // 10 bit pow2
        // .duty_references = {0, 585, 1170, 1755, 2340, 2925, 3510, 4095}, // 12 bit linear
        .duty_references = {0, 80, 329, 668, 1324, 2048, 2992, 4095}, // 12 bit pow2
        // .duty_references = {4095, 2992, 2048, 1324, 668, 329, 80, 0}, // 12 bit pow2 invert
        .brightness = 255,
        .color = {.r = 0, .g = 0, .b = 0},
    };
    rgb_ledc_config(&rgb_ledc);
}

void init()
{
    printf("Initialization...\n");

    init_rgb_led();

    printf("Initialization completed!\n");
}

int32_t sign(int32_t x)
{
    if (x > 0)
        return 1;
    if (x < 0)
        return -1;
    return 0;
}

void make_rainbow()
{
    rgb_t color = {.r = 0, .g = 0, .b = 0};
    int16_t step = 5;

    while (true)
    {
        if (color.g > 0 && color.b == 255u && color.r == 0)
            color.g -= step;
        else if (color.b > 0 && color.r == 255u && color.g == 0)
            color.b -= step;
        else if (color.r > 0 && color.g == 255u && color.b == 0)
            color.r -= step;
        else if (color.b < 255u && color.g == 255u && color.r == 0)
            color.b += step;
        else if (color.g < 255u && color.r == 255u && color.b == 0)
            color.g += step;
        else
            color.r += step;

        rgb_ledc_set_color(RGB_LED_0, color);
        delay_ms(20);
    }
}

void make_random_color_smooth_blinking()
{
    rgb_t color = {0};
    uint8_t brightness = 0;
    uint8_t stage = 0;

    rgb_ledc_set_color_and_brightness(RGB_LED_0, color, brightness);

    while (true)
    {
        if (stage == 0)
        {
            color = rnd_rgb();
            brightness = 0;
            printf("Color: #%02x%02x%02x\n", color.r, color.g, color.b);
            stage++;
        }
        else if (stage == 1)
        {
            brightness++;
            if (brightness == 255)
                stage++;
        }
        else if (stage == 2)
        {
            brightness--;
            if (brightness == 0)
                stage++;
        }
        else
        {
            stage = 0;
        }

        rgb_ledc_set_color_and_brightness(RGB_LED_0, color, brightness);
        delay_ms(30);
    }
}

void make_random_color_smooth_transition()
{
    rgb_t target_color = {0};
    rgb_t current_color = {0};
    rgbx_t diff = {0};
    uint8_t brightness = 255;

    rgb_ledc_set_color_and_brightness(RGB_LED_0, current_color, brightness);

    while (true)
    {
        if (cmp_rgb(current_color, target_color) == 0)
        {
            target_color = rnd_colorful_rgb();
            printf("Target color: #%02x%02x%02x\n", target_color.r, target_color.g, target_color.b);
        }
        else
        {
            diff = sub_rgb(target_color, current_color);
            current_color.r += sign(diff.r);
            current_color.g += sign(diff.g);
            current_color.b += sign(diff.b);
        }

        rgb_ledc_set_color_and_brightness(RGB_LED_0, current_color, brightness);
        delay_ms(30);
    }
}

void app_main(void)
{
    init();

    // make_random_color_smooth_blinking();
    make_random_color_smooth_transition();
}
