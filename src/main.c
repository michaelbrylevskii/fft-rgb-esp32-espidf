#include <stdio.h>

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
        .duty_references = {0, 80, 329, 668, 1324, 2048, 2992, 4095}, // 12 bit pow2
        // .duty_references = {0, 585, 1170, 1755, 2340, 2925, 3510, 4095}, // 12 bit linear
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

void make_brightness_test()
{
    rgb_t color = {.r = 0, .g = 0, .b = 0};
    rgb_t rnd = {.r = 0, .g = 0, .b = 0};
    uint8_t brightness = 0;
    uint8_t stage = 0;

    rgb_ledc_set_color(RGB_LED_0, color);
    rgb_ledc_set_brightness(RGB_LED_0, brightness);

    while (true)
    {
        if (stage == 0)
        {
            esp_fill_random(&color, sizeof(color));
            esp_fill_random(&rnd, sizeof(rnd));
            brightness = 0;

            if (rnd.r < 30)
                color.r = 0;
            if (rnd.g < 30)
                color.g = 0;
            if (rnd.b < 30)
                color.b = 0;

            if (rnd.r > 255 - 30)
                color.r = 255;
            if (rnd.g > 255 - 30)
                color.g = 255;
            if (rnd.b > 255 - 30)
                color.b = 255;

            printf("Color: #%x%x%x\n", color.r, color.g, color.b);
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
        delay_ms(20);
    }
}

void app_main(void)
{
    init();

    make_brightness_test();
}
