/**
 * @file fontawesome_font.h
 * @brief Font Awesome font declaration for LVGL
 */

#ifndef FONTAWESOME_FONT_H
#define FONTAWESOME_FONT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <lvgl.h>

/* Font Awesome Font Declaration */
LV_FONT_DECLARE(lv_font_fontawesome_16);

/* Font macro for easy use */
#define FONT_FONTAWESOME    &lv_font_fontawesome_16

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*FONTAWESOME_FONT_H*/
