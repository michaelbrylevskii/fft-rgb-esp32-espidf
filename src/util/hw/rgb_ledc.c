#include "util/hw/rgb_ledc.h"

#include <stdlib.h>
#include <string.h>
#include "util/stopwatch.h" // TODO: remove it

static rgb_ledc_config_t *_rgb_ledc_array[RGB_LED_MAX];

static void _rgb_ledc_config_ledc(const rgb_ledc_config_t *config)
{
    // Timer config
    ledc_timer_config_t ledc_timer = {
        .timer_num = config->timer,
        .speed_mode = config->speed_mode,
        .duty_resolution = config->duty_resolution,
        .freq_hz = config->freq_hz};
    ERROR(ledc_timer_config(&ledc_timer));

    // Red channel config
    ledc_channel_config_t ledc_channel_r = {
        .channel = config->channel_r,
        .gpio_num = config->gpio_r,
        .speed_mode = config->speed_mode,
        .timer_sel = config->timer};
    ERROR(ledc_channel_config(&ledc_channel_r));

    // Green channel config
    ledc_channel_config_t ledc_channel_g = {
        .channel = config->channel_g,
        .gpio_num = config->gpio_g,
        .speed_mode = config->speed_mode,
        .timer_sel = config->timer};
    ERROR(ledc_channel_config(&ledc_channel_g));

    // Blue channel config
    ledc_channel_config_t ledc_channel_b = {
        .channel = config->channel_b,
        .gpio_num = config->gpio_b,
        .speed_mode = config->speed_mode,
        .timer_sel = config->timer};
    ERROR(ledc_channel_config(&ledc_channel_b));
}

static rgb_ledc_config_t *_rgb_ledc_init_config_slot(const rgb_ledc_config_t *config)
{
    rgb_ledc_config_t *local_config = _rgb_ledc_array[config->led];

    if (local_config != NULL)
    {
        // Already configured
        ERROR_INVALID_STATE;
        return NULL;
    }

    local_config = malloc(sizeof(rgb_ledc_config_t));
    if (local_config == NULL)
    {
        // Couldn't allocate memory
        ERROR_NO_MEM;
        return NULL;
    }

    memcpy(local_config, config, sizeof(rgb_ledc_config_t));
    _rgb_ledc_array[config->led] = local_config;
    return local_config;
}

static rgb_ledc_config_t *_rgb_ledc_get_config_slot(rgb_ledc_led_t led)
{
    rgb_ledc_config_t *local_config = _rgb_ledc_array[led];

    if (local_config == NULL)
    {
        // Not configured yet
        ERROR_INVALID_STATE;
        return NULL;
    }

    return local_config;
}

uint16_t _rgb_ledc_calc_duty(uint8_t color, const uint16_t duty_references[])
{
    uint8_t index = (color * RGB_LEDC_DUTY_REFERENCE_LAST) / 255;
    uint8_t next_index = index + 1;
    if (next_index > RGB_LEDC_DUTY_REFERENCE_LAST)
        next_index = RGB_LEDC_DUTY_REFERENCE_LAST;

    int16_t start_duty = duty_references[index];
    int16_t end_duty = duty_references[next_index];
    int16_t fraction = ((color * RGB_LEDC_DUTY_REFERENCE_LAST) % 255) * 1000 / 255;

    return start_duty + ((end_duty - start_duty) * fraction / 1000);
}

static void _rgb_ledc_update_duty(const rgb_ledc_config_t *config)
{
    rgb_t color = config->color;
    uint8_t brightness = config->brightness; // TODO: use this

    STOPWATCH_START;
    uint16_t duty_r = _rgb_ledc_calc_duty(color.r, config->duty_references);
    uint16_t duty_g = _rgb_ledc_calc_duty(color.g, config->duty_references);
    uint16_t duty_b = _rgb_ledc_calc_duty(color.b, config->duty_references);
    STOPWATCH_STOP;
    STOPWATCH_PRINT(10000, "_rgb_ledc_calc_duty");

    ERROR(ledc_set_duty(config->speed_mode, config->channel_r, duty_r));
    ERROR(ledc_set_duty(config->speed_mode, config->channel_g, duty_g));
    ERROR(ledc_set_duty(config->speed_mode, config->channel_b, duty_b));
    ERROR(ledc_update_duty(config->speed_mode, config->channel_r));
    ERROR(ledc_update_duty(config->speed_mode, config->channel_g));
    ERROR(ledc_update_duty(config->speed_mode, config->channel_b));
}

void rgb_ledc_config(const rgb_ledc_config_t *config)
{
    if (config->duty_resolution > LEDC_TIMER_12_BIT)
    {
        ERROR_NOT_SUPPORTED;
        return;
    }

    rgb_ledc_config_t *local_config = _rgb_ledc_init_config_slot(config);
    _rgb_ledc_config_ledc(local_config);
    _rgb_ledc_update_duty(local_config);
}

uint8_t rgb_ledc_get_brightness(rgb_ledc_led_t led)
{
    rgb_ledc_config_t *local_config = _rgb_ledc_get_config_slot(led);
    return local_config->brightness;
}

void rgb_ledc_set_brightness(rgb_ledc_led_t led, uint8_t brightness)
{
    rgb_ledc_config_t *local_config = _rgb_ledc_get_config_slot(led);
    local_config->brightness = brightness;
    _rgb_ledc_update_duty(local_config);
}

rgb_t rgb_ledc_get_color(rgb_ledc_led_t led)
{
    rgb_ledc_config_t *local_config = _rgb_ledc_get_config_slot(led);
    return local_config->color;
}

void rgb_ledc_set_color(rgb_ledc_led_t led, rgb_t color)
{
    rgb_ledc_config_t *local_config = _rgb_ledc_get_config_slot(led);
    local_config->color = color;
    _rgb_ledc_update_duty(local_config);
}
