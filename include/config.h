// => Hardware select
// Note: LILYGO_LILYPI_V1 is defined in platformio.ini build_flags
#ifndef LILYGO_LILYPI_V1
#define LILYGO_LILYPI_V1
#endif

//NOT SUPPORT ...
// #define LILYGO_WATCH_2020_V1
// #define LILYGO_WATCH_2019_WITH_TOUCH
// #define LILYGO_WATCH_2019_NO_TOUCH
// #define LILYGO_WATCH_BLOCK

//NOT SUPPORT ...
// => Function select
// Note: LILYGO_BLOCK_ILI9481_MODULE is defined in platformio.ini build_flags
#ifndef LILYGO_BLOCK_ILI9481_MODULE
#define LILYGO_BLOCK_ILI9481_MODULE         //Use ILI9481 - CORRECT FOR THIS HARDWARE
#endif
// #define LILYGO_BLOCK_ST7796S_MODULE          //Use ST7796S

// Include LilyGoWatch library which will include board definitions
// Note: LILYGO_WATCH_LVGL is now defined in platformio.ini build_flags
#include <LilyGoWatch.h>
