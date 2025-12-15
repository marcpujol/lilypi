/**
 * @file fontawesome_icons.h
 * @brief Font Awesome 6 icon definitions for LVGL with actual Unicode characters
 */

#ifndef FONTAWESOME_ICONS_H
#define FONTAWESOME_ICONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <lvgl.h>
#include "fontawesome_font.h"

/* Font Awesome 6 Icon Codes with actual Unicode characters */
#define FA_WIFI              "\uf1eb"    // U+F1EB
#define FA_WIFI_SLASH        "\uf1eb"    // U+F1EB (using same as wifi)
#define FA_SIGNAL            "\uf1eb"    // U+F1EB (using wifi)
#define FA_ARROW_UP          "\uf062"    // U+F062
#define FA_ARROW_DOWN        "\uf063"    // U+F063
#define FA_ARROW_LEFT        "\uf060"    // U+F060
#define FA_ARROW_RIGHT       "\uf061"    // U+F061
#define FA_CARET_UP          "\uf062"    // U+F062 (using arrow up)
#define FA_CARET_DOWN        "\uf063"    // U+F063 (using arrow down)
#define FA_CHEVRON_UP        "\uf077"    // U+F077
#define FA_CHEVRON_DOWN      "\uf078"    // U+F078
#define FA_CHECK             "\uf00c"    // U+F00C
#define FA_TIMES             "\uf00d"    // U+F00D
#define FA_SAVE              "\uf0c7"    // U+F0C7
#define FA_CANCEL            "\uf00d"    // U+F00D (using times)
#define FA_SETTINGS          "\uf013"    // U+F013
#define FA_COG               "\uf013"    // U+F013
#define FA_HOME              "\uf015"    // U+F015
#define FA_THERMOMETER       "\uf2c7"    // U+F2C7
#define FA_DROPLET           "\uf043"    // U+F043
#define FA_TINT              "\uf043"    // U+F043
#define FA_FIRE              "\uf06d"    // U+F06D
#define FA_FLAME             "\uf06d"    // U+F06D
#define FA_FAUCET            "\uf043"    // U+F043 (using droplet)
#define FA_PUMP_SOAP         "\uf043"    // U+F043 (using droplet)
#define FA_WATER             "\uf043"    // U+F043 (using droplet)
#define FA_CLOCK             "\uf017"    // U+F017
#define FA_STOPWATCH         "\uf017"    // U+F017 (using clock)
#define FA_POWER_OFF         "\uf011"    // U+F011
#define FA_TOGGLE_ON         "\uf00c"    // U+F00C (using check)
#define FA_TOGGLE_OFF        "\uf00d"    // U+F00D (using times)

/* Icon character codes as integers (kept for compatibility) */
#define FA_WIFI_CODE              0xf1eb
#define FA_WIFI_SLASH_CODE        0xf1ac
#define FA_SIGNAL_CODE            0xf012
#define FA_ARROW_UP_CODE          0xf062
#define FA_ARROW_DOWN_CODE        0xf063
#define FA_ARROW_LEFT_CODE        0xf060
#define FA_ARROW_RIGHT_CODE       0xf061
#define FA_CARET_UP_CODE          0xf0d8
#define FA_CARET_DOWN_CODE        0xf0d7
#define FA_CHEVRON_UP_CODE        0xf077
#define FA_CHEVRON_DOWN_CODE      0xf078
#define FA_CHECK_CODE             0xf00c
#define FA_TIMES_CODE             0xf00d
#define FA_SAVE_CODE              0xf0c7
#define FA_CANCEL_CODE            0xf05e
#define FA_SETTINGS_CODE          0xf013
#define FA_COG_CODE               0xf013
#define FA_HOME_CODE              0xf015
#define FA_THERMOMETER_CODE       0xf2c7
#define FA_DROPLET_CODE           0xf043
#define FA_TINT_CODE              0xf043
#define FA_FIRE_CODE              0xf06d
#define FA_FLAME_CODE             0xf06d
#define FA_FAUCET_CODE            0xe005
#define FA_PUMP_SOAP_CODE         0xe06b
#define FA_WATER_CODE             0xf773
#define FA_CLOCK_CODE             0xf017
#define FA_STOPWATCH_CODE         0xf2f2
#define FA_POWER_OFF_CODE         0xf011
#define FA_TOGGLE_ON_CODE         0xf205
#define FA_TOGGLE_OFF_CODE        0xf204

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*FONTAWESOME_ICONS_H*/
