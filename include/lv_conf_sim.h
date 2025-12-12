/**
 * @file lv_conf.h
 * Configuration file for LVGL v7.11.0
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/* Color settings */
#define LV_COLOR_DEPTH     16
#define LV_COLOR_16_SWAP   0

/* Memory settings */
#define LV_MEM_CUSTOM      0
#define LV_MEM_SIZE        (32U * 1024U)

/* HAL settings */
#define LV_TICK_CUSTOM     0
#define LV_DPI             100

/* Feature usage */
#define LV_USE_GPU         0
#define LV_USE_FILESYSTEM  0
#define LV_USE_USER_DATA   1

/* Compiler settings */
#define LV_ATTRIBUTE_TICK_INC
#define LV_ATTRIBUTE_TASK_HANDLER
#define LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_LARGE_CONST
#define LV_ATTRIBUTE_FAST_MEM

/* Enable/disable built-in features */
#define LV_USE_ANIMATION   1
#define LV_USE_SHADOW      1
#define LV_USE_BLEND_MODES 1
#define LV_USE_OPA_SCALE   1
#define LV_USE_IMG_TRANSFORM 1
#define LV_USE_GROUP       1
#define LV_USE_GPU_STM32_DMA2D 0
#define LV_USE_GPU_NXP_PXP 0
#define LV_USE_GPU_NXP_VG_LITE 0
#define LV_USE_EXTERNAL_RENDERER 0

/* Logging */
#define LV_USE_LOG      1
#define LV_LOG_LEVEL    LV_LOG_LEVEL_WARN
#define LV_LOG_PRINTF   1

/* Debug settings */
#define LV_USE_DEBUG    0
#define LV_USE_ASSERT_NULL      1
#define LV_USE_ASSERT_MALLOC    1
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ       0
#define LV_USE_ASSERT_STYLE     0

/* Objects usage */
#define LV_USE_ARC          1
#define LV_USE_BAR          1
#define LV_USE_BTN          1
#define LV_USE_BTNMATRIX    1
#define LV_USE_CANVAS       1
#define LV_USE_CHECKBOX     1
#define LV_USE_CONT         1
#define LV_USE_CPICKER      1
#define LV_USE_DROPDOWN     1
#define LV_USE_GAUGE        1
#define LV_USE_IMG          1
#define LV_USE_IMGBTN       1
#define LV_USE_KEYBOARD     1
#define LV_USE_LABEL        1
#define LV_USE_LED          1
#define LV_USE_LINE         1
#define LV_USE_LIST         1
#define LV_USE_LINEMETER    1
#define LV_USE_OBJMASK      1
#define LV_USE_MSGBOX       1
#define LV_USE_PAGE         1
#define LV_USE_SPINNER      1
#define LV_USE_ROLLER       1
#define LV_USE_SLIDER       1
#define LV_USE_SPINBOX      1
#define LV_USE_SWITCH       1
#define LV_USE_TEXTAREA     1
#define LV_USE_TABLE        1
#define LV_USE_TABVIEW      1
#define LV_USE_TILEVIEW     1
#define LV_USE_WIN          1
#define LV_USE_CALENDAR     1
#define LV_USE_CHART        1

/* Themes */
#define LV_USE_THEME_EMPTY       0
#define LV_USE_THEME_TEMPLATE    0
#define LV_USE_THEME_MATERIAL    1
#define LV_USE_THEME_MONO        0

#define LV_THEME_DEFAULT_INIT               lv_theme_material_init
#define LV_THEME_DEFAULT_COLOR_PRIMARY      lv_color_hex(0x01a2b1)
#define LV_THEME_DEFAULT_COLOR_SECONDARY    lv_color_hex(0x44d62c)
#define LV_THEME_DEFAULT_FLAG               LV_THEME_MATERIAL_FLAG_LIGHT
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_14
#define LV_THEME_DEFAULT_FONT_NORMAL        &lv_font_montserrat_14
#define LV_THEME_DEFAULT_FONT_SUBTITLE      &lv_font_montserrat_14
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_14

/* Font usage */
#define LV_FONT_MONTSERRAT_8     0
#define LV_FONT_MONTSERRAT_10    0
#define LV_FONT_MONTSERRAT_12    0
#define LV_FONT_MONTSERRAT_14    1
#define LV_FONT_MONTSERRAT_16    1
#define LV_FONT_MONTSERRAT_18    0
#define LV_FONT_MONTSERRAT_20    0
#define LV_FONT_MONTSERRAT_22    0
#define LV_FONT_MONTSERRAT_24    0
#define LV_FONT_MONTSERRAT_26    0
#define LV_FONT_MONTSERRAT_28    0
#define LV_FONT_MONTSERRAT_30    0
#define LV_FONT_MONTSERRAT_32    0
#define LV_FONT_MONTSERRAT_34    0
#define LV_FONT_MONTSERRAT_36    0
#define LV_FONT_MONTSERRAT_38    0
#define LV_FONT_MONTSERRAT_40    0
#define LV_FONT_MONTSERRAT_42    0
#define LV_FONT_MONTSERRAT_44    0
#define LV_FONT_MONTSERRAT_46    0
#define LV_FONT_MONTSERRAT_48    0

#define LV_FONT_UNSCII_8     0
#define LV_FONT_UNSCII_16    0

#define LV_FONT_CUSTOM_DECLARE

#define LV_FONT_DEFAULT        &lv_font_montserrat_14

/* Text settings */
#define LV_TXT_ENC LV_TXT_ENC_UTF8
#define LV_TXT_BREAK_CHARS   " ,.;:-_"

/* LV_TXT_LINE_BREAK_LONG_LEN - Wrap texts longer than this value */
#define LV_TXT_LINE_BREAK_LONG_LEN 12
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN 3
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 1

/* Compiler attributes */
#define LV_ATTRIBUTE_EXTERN_DATA
#define LV_ATTRIBUTE_FLUSH_CACHE(var, len)
#define LV_ATTRIBUTE_FAST_MEM_INIT

#endif /*LV_CONF_H*/
