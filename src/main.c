#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#include "config.h"
#include "util/error.h"
#include "util/hw/rgb_ledc.h"

void init_rgb_led()
{
    // Timer config
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_10_BIT,
        .freq_hz = 2000,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0};

    // Channels config
    ledc_channel_config_t ledc_channel_r = {
        .channel = LEDC_CHANNEL_0,
        .gpio_num = LED_R_GPIO,
        .speed_mode = ledc_timer.speed_mode,
        .timer_sel = ledc_timer.timer_num};
    ledc_channel_config_t ledc_channel_g = {
        .channel = LEDC_CHANNEL_1,
        .gpio_num = LED_G_GPIO,
        .speed_mode = ledc_timer.speed_mode,
        .timer_sel = ledc_timer.timer_num};
    ledc_channel_config_t ledc_channel_b = {
        .channel = LEDC_CHANNEL_2,
        .gpio_num = LED_B_GPIO,
        .speed_mode = ledc_timer.speed_mode,
        .timer_sel = ledc_timer.timer_num};

    ERROR(ledc_timer_config(&ledc_timer));
    ERROR(ledc_channel_config(&ledc_channel_r));
    ERROR(ledc_channel_config(&ledc_channel_g));
    ERROR(ledc_channel_config(&ledc_channel_b));

    rgb_ledc_config_t rgb_ledc = {
        .led = LED_0,
        .channel_r = ledc_channel_r.channel,
        .channel_g = ledc_channel_g.channel,
        .channel_b = ledc_channel_b.channel,
        .speed_mode = ledc_timer.speed_mode,
        .duty_resolution = ledc_timer.duty_resolution,
        .scale = LINEAR,
        .brightness = 0,
        .color = (rgb_t){.r = 0, .g = 128, .b = 0}};
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

        rgb_ledc_set_color(LED_0, color);
        delay_ms(10);
        // rgb_ledc_get_color(LED_1);
    }
}
