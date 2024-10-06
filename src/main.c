#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#include "config.h"
#include "util/error.h"
#include "util/hw/rgb_ledc.h"

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
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .freq_hz = 2900,
        .scale = LINEAR,
        .brightness = 255,
        .color = (rgb_t){{0, 128, 0}},
    };
    rgb_ledc_config(&rgb_ledc);
}

void init()
{
    printf("Initialization...\n");

    init_rgb_led();

    printf("Initialization completed!\n");
}

void delay_ms(uint16_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void app_main(void)
{
    init();

    // printf("1 << 0 = %u\n", 1u << 0);
    // printf("1 << 1 = %u\n", 1u << 1);
    // printf("1 << 2 = %u\n", 1u << 2);
    // printf("1 << 3 = %u\n", 1u << 3);
    // printf("1 << 4 = %u\n", 1u << 4);

    rgb_t color = {{0, 0, 0}};
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
        delay_ms(10);
        // rgb_ledc_get_color(RGB_LED_1);
    }
}
