/**
 * @file lightbulb.h
 * @brief Lightbulb icon control for LVGL GUI
 */

#ifndef LIGHTBULB_H
#define LIGHTBULB_H

#ifdef __cplusplus
extern "C" {
#endif

// Note: This header requires lvgl.h to be included before it
// The parent file should include lvgl/lvgl.h first

// Initialize and create lightbulb icon on given screen
void lightbulb_create(void* parent_screen, void* icon_font);

// Control lightbulb state externally
void lightbulb_on(void);
void lightbulb_off(void);
bool lightbulb_get_state(void);

#ifdef __cplusplus
}
#endif

#endif // LIGHTBULB_H
