#pragma once

#include <stdint.h>
#include "driver/gpio.h"
#include "driver/ledc.h"

#include "config.h"
#include "util/error.h"
#include "util/color.h"

#ifndef RGB_LEDC_DUTY_REFERENCES_COUNT
#define RGB_LEDC_DUTY_REFERENCES_COUNT 8
#endif
#define RGB_LEDC_DUTY_REFERENCE_LAST (RGB_LEDC_DUTY_REFERENCES_COUNT - 1)

typedef enum
{
    RGB_LED_0 = 0,
    RGB_LED_1,
    RGB_LED_2,
    RGB_LED_MAX
} rgb_ledc_led_t;

typedef struct
{
    rgb_ledc_led_t led;
    ledc_timer_t  timer;
    ledc_channel_t channel_r;
    ledc_channel_t channel_g;
    ledc_channel_t channel_b;
    gpio_num_t gpio_r;
    gpio_num_t gpio_g;
    gpio_num_t gpio_b;
    uint16_t freq_hz;
    ledc_mode_t speed_mode;
    ledc_timer_bit_t duty_resolution;
    uint16_t duty_references[RGB_LEDC_DUTY_REFERENCES_COUNT];
    uint8_t brightness;
    rgb_t color;
} rgb_ledc_config_t;

void rgb_ledc_config(const rgb_ledc_config_t *config);

uint8_t rgb_ledc_get_brightness(rgb_ledc_led_t led);

void rgb_ledc_set_brightness(rgb_ledc_led_t led, uint8_t brightness);

rgb_t rgb_ledc_get_color(rgb_ledc_led_t led);

void rgb_ledc_set_color(rgb_ledc_led_t led, rgb_t color);
