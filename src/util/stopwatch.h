#pragma once

#include <stdint.h>
#include <stdio.h>

#include "esp_timer.h"

#define STOPWATCH_START               \
    static int64_t __total_time = 0;  \
    static int32_t __total_count = 0; \
    int64_t __start_time = esp_timer_get_time();

#define STOPWATCH_STOP                         \
    int64_t __end_time = esp_timer_get_time(); \
    __total_time += __end_time - __start_time; \
    __total_count++;

#define STOPWATCH_PRINT(__each_times, __name)                                                  \
    if (__total_count >= __each_times)                                                         \
    {                                                                                          \
        printf("Time for %ld times for '%s': %lld us\n", __total_count, __name, __total_time); \
        __total_time = 0;                                                                      \
        __total_count = 0;                                                                     \
    }
