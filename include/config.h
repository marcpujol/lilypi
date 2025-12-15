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

// WiFi Configuration
#define WIFI_SSID "MapuDevice"
#define WIFI_PASSWORD "uni3xtr4!X"
#define WIFI_MAX_CONNECT_ATTEMPTS 3
#define WIFI_CONNECT_TIMEOUT_MS 20000

// Network Services Configuration
#define NTP_SERVER "europe.pool.ntp.org"
#define GMT_OFFSET_SEC 3600          // Madrid is UTC+1
#define DAYLIGHT_OFFSET_SEC 3600     // Daylight saving time

// MQTT Configuration
#define MQTT_SERVER "192.168.1.27"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "garage-controller"

// Relay GPIO Configuration
#define GPIO_RELAY1 40
#define GPIO_RELAY2 2
#define GPIO_RELAY3 1
