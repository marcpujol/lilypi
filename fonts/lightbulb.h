/**
 * @file lightbulb.h
 * @brief Lightbulb icon control for LVGL GUI
 */

#ifndef LIGHTBULB_H
#define LIGHTBULB_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Initialize and create lightbulb icon on given screen
void lightbulb_create(lv_obj_t* parent_screen, lv_font_t* icon_font);

// Control lightbulb state externally
void lightbulb_on(void);
void lightbulb_off(void);
bool lightbulb_get_state(void);

#ifdef __cplusplus
}
#endif

#endif // LIGHTBULB_H
