#include "util/hw/rgb_ledc.h"

#include <string.h>
#include "util/error.h"

rgb_ledc_config_t *_rgb_ledc_array[RGB_LEDC_MAX_COUNT];

void rgb_ledc_config(const rgb_ledc_config_t *config)
{
    rgb_ledc_config_t *local_config = _rgb_ledc_array[config->led];

    if (local_config == NULL)
    {
        local_config = malloc(sizeof(rgb_ledc_config_t));
        if (local_config == NULL)
        {
            ERROR_NO_MEM;
            return;
        }
    }

    memcpy(local_config, config, sizeof(rgb_ledc_config_t));
    _rgb_ledc_array[config->led] = local_config;

    if (local_config->duty_max == 0)
    {
        local_config->duty_max = 1u << local_config->duty_resolution;
    }

    rgb_ledc_set_color(local_config->led, local_config->color);
}

uint8_t rgb_ledc_get_brightness(rgb_ledc_led_t led)
{
    // TODO
    return 0;
}

void rgb_ledc_set_brightness(rgb_ledc_led_t led, uint8_t brightness)
{
    // TODO
}

rgb_t rgb_ledc_get_color(rgb_ledc_led_t led)
{
    rgb_ledc_config_t *config = _rgb_ledc_array[led];

    if (config == NULL)
    {
        ERROR_INVALID_ARG;
        return (rgb_t){{0, 0, 0}};
    }

    return config->color;
}

void rgb_ledc_set_color(rgb_ledc_led_t led, rgb_t color)
{
    rgb_ledc_config_t *config = _rgb_ledc_array[led];

    if (config == NULL)
    {
        ERROR_INVALID_ARG;
        return;
    }

    config->color = color;

    uint32_t duty_r;
    uint32_t duty_g;
    uint32_t duty_b;

    if (config->scale == LINEAR)
    {
        uint32_t factor = config->duty_max / 255u;
        duty_r = color.r * factor;
        duty_g = color.g * factor;
        duty_b = color.b * factor;
    }
    else
    {
        ERROR_NOT_SUPPORTED;
        return;
    }

    ERROR(ledc_set_duty(config->speed_mode, config->channel_r, duty_r));
    ERROR(ledc_set_duty(config->speed_mode, config->channel_g, duty_g));
    ERROR(ledc_set_duty(config->speed_mode, config->channel_b, duty_b));
    ERROR(ledc_update_duty(config->speed_mode, config->channel_r));
    ERROR(ledc_update_duty(config->speed_mode, config->channel_g));
    ERROR(ledc_update_duty(config->speed_mode, config->channel_b));
}
