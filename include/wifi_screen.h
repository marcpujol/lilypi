#ifndef WIFI_SCREEN_H
#define WIFI_SCREEN_H

#include "lvgl/lvgl.h"

// Initialize and show the WiFi connection screen
void wifi_screen_create(void);

// Hide the WiFi screen and show the main UI
void wifi_screen_hide(void);

// Update the WiFi connection status message
void wifi_screen_update_status(const char* message);

#endif // WIFI_SCREEN_H
