/**
 * @file hal.h
 * Hardware Abstraction Layer for SDL2
 */

#ifndef HAL_H
#define HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lv_conf_sim.h"
#include "lvgl.h"

/**
 * Initialize the HAL (display, input devices, tick) for LVGL
 * @param width Display width in pixels
 * @param height Display height in pixels
 */
void sdl_hal_init(int32_t width, int32_t height);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* HAL_H */
