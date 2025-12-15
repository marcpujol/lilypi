#include "lvgl/lvgl.h"
#include <stdio.h>
#include <string.h>

// Declare lightbulb functions with C linkage
extern "C" {
    void lightbulb_on(void);
    void lightbulb_off(void);
    bool lightbulb_get_state(void);
}

// Declare FontAwesome fonts
LV_FONT_DECLARE(lv_font_fontawesome_16);
LV_FONT_DECLARE(lv_font_fontawesome_32);

// Declare Montserrat fonts for temperature values
LV_FONT_DECLARE(lv_font_montserrat_22);

extern void setBrightness(uint8_t level);
extern void relayTurnOff(void);
extern void relayTurnOn(void);

// Declare MQTT publish function
extern "C" {
    bool mqtt_publish_command(const char* topic, const char* payload);
}

static void brightness_slider_event_cb(lv_obj_t *slider, lv_event_t e);
static void garage_btn_event_cb(lv_obj_t *obj, lv_event_t e);
static void entrance_switch_event_cb(lv_obj_t *sw, lv_event_t e);
static void catdoor_switch_event_cb(lv_obj_t *sw, lv_event_t e);
static void update_lightbulb_color(lv_obj_t *label, bool is_on);
static void update_button_background(lv_obj_t *btn, bool is_active);
static void update_stop_button_state(void);
static void update_up_down_buttons(lv_obj_t *active_btn);

// Global state variables
static bool g_stop_active = false;
static bool g_up_active = false;
static bool g_down_active = false;

// Global references to buttons and switches for state updates
static lv_obj_t *g_lightbulb_label = NULL;
static lv_obj_t *g_up_btn = NULL;
static lv_obj_t *g_stop_btn = NULL;
static lv_obj_t *g_down_btn = NULL;
static lv_obj_t *g_entrance_switch = NULL;
static lv_obj_t *g_catdoor_switch = NULL;
static lv_obj_t *g_outdoor_temp_label = NULL;
static lv_obj_t *g_indoor_temp_label = NULL;

// Styles for lightbulb on/off states
static lv_style_t style_lightbulb_on;
static lv_style_t style_lightbulb_off;

// Styles for garage button states
static lv_style_t style_button_active;
static lv_style_t style_button_inactive;

void lv_demo_widgets(void)
{
    // Declare variables at the beginning (C requirement)
    int row_height;
    int panel_width;

    // Create LEFT panel - "garage" (50% width) - full height, no top bar
    lv_obj_t *garage_panel = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_set_size(garage_panel, LV_HOR_RES / 2 - 5, LV_VER_RES);
    lv_obj_set_pos(garage_panel, 0, 0);
    lv_cont_set_layout(garage_panel, LV_LAYOUT_CENTER);

    // Remove panel background and borders
    static lv_style_t style_transparent;
    lv_style_init(&style_transparent);
    lv_style_set_bg_opa(&style_transparent, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_style_set_border_width(&style_transparent, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_all(&style_transparent, LV_STATE_DEFAULT, 0);
    lv_obj_add_style(garage_panel, LV_CONT_PART_MAIN, &style_transparent);

    // Style for garage buttons with rounded borders - set border for all states
    static lv_style_t style_garage_btn;
    lv_style_init(&style_garage_btn);
    lv_style_set_radius(&style_garage_btn, LV_STATE_DEFAULT, 15);
    lv_style_set_border_width(&style_garage_btn, LV_STATE_DEFAULT, 2);
    lv_style_set_border_color(&style_garage_btn, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_border_width(&style_garage_btn, LV_STATE_PRESSED, 2);
    lv_style_set_border_color(&style_garage_btn, LV_STATE_PRESSED, LV_COLOR_WHITE);
    lv_style_set_border_width(&style_garage_btn, LV_STATE_FOCUSED, 2);
    lv_style_set_border_color(&style_garage_btn, LV_STATE_FOCUSED, LV_COLOR_WHITE);
    lv_style_set_border_width(&style_garage_btn, LV_STATE_CHECKED, 2);
    lv_style_set_border_color(&style_garage_btn, LV_STATE_CHECKED, LV_COLOR_WHITE);
    // Disable the outline (focus indicator) to prevent turquoise border
    lv_style_set_outline_width(&style_garage_btn, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&style_garage_btn, LV_STATE_FOCUSED, 0);
    lv_style_set_pad_all(&style_garage_btn, LV_STATE_DEFAULT, 20);

    // Create container for vertically stacked buttons
    lv_obj_t *btn_cont = lv_cont_create(garage_panel, NULL);
    lv_cont_set_layout(btn_cont, LV_LAYOUT_COLUMN_MID);
    lv_cont_set_fit(btn_cont, LV_FIT_TIGHT);
    lv_obj_add_style(btn_cont, LV_CONT_PART_MAIN, &style_transparent);

    // Initialize button state styles for button backgrounds
    lv_style_init(&style_button_active);
    lv_style_set_bg_color(&style_button_active, LV_STATE_DEFAULT, lv_color_hex(0xE0E0E0));  // Very light gray when active
    lv_style_set_radius(&style_button_active, LV_STATE_DEFAULT, 15);
    lv_style_set_border_width(&style_button_active, LV_STATE_DEFAULT, 2);
    lv_style_set_border_color(&style_button_active, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_pad_all(&style_button_active, LV_STATE_DEFAULT, 20);
    
    lv_style_init(&style_button_inactive);
    lv_style_set_bg_color(&style_button_inactive, LV_STATE_DEFAULT, LV_COLOR_WHITE);  // White when inactive
    lv_style_set_radius(&style_button_inactive, LV_STATE_DEFAULT, 15);
    lv_style_set_border_width(&style_button_inactive, LV_STATE_DEFAULT, 2);
    lv_style_set_border_color(&style_button_inactive, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_pad_all(&style_button_inactive, LV_STATE_DEFAULT, 20);

    // Style for button text (dark gray for visibility on white background)
    static lv_style_t style_button_text;
    lv_style_init(&style_button_text);
    lv_style_set_text_font(&style_button_text, LV_STATE_DEFAULT, &lv_font_montserrat_32);
    lv_style_set_text_color(&style_button_text, LV_STATE_DEFAULT, lv_color_hex(0x2A2A2A));  // Dark gray

    // Button 1: Arrow UP (LV_SYMBOL_UP) - optimized for vertical fit
    lv_obj_t *btn1 = lv_btn_create(btn_cont, NULL);
    lv_obj_set_size(btn1, 126, 50);
    lv_obj_add_style(btn1, LV_BTN_PART_MAIN, &style_garage_btn);
    lv_obj_add_style(btn1, LV_BTN_PART_MAIN, &style_button_inactive);  // Set initial background
    lv_obj_set_event_cb(btn1, garage_btn_event_cb);
    lv_obj_t *label1 = lv_label_create(btn1, NULL);
    lv_label_set_text(label1, LV_SYMBOL_UP);
    lv_obj_add_style(label1, LV_LABEL_PART_MAIN, &style_button_text);
    g_up_btn = btn1;  // Store button reference

    // Style for STOP button text (red color)
    static lv_style_t style_stop_text;
    lv_style_init(&style_stop_text);
    lv_style_set_text_font(&style_stop_text, LV_STATE_DEFAULT, &lv_font_montserrat_32);
    lv_style_set_text_color(&style_stop_text, LV_STATE_DEFAULT, lv_color_hex(0xB81509));  // Red

    // Button 2: STOP (LV_SYMBOL_STOP) - optimized for vertical fit
    lv_obj_t *btn2 = lv_btn_create(btn_cont, NULL);
    lv_obj_set_size(btn2, 126, 50);
    lv_obj_add_style(btn2, LV_BTN_PART_MAIN, &style_garage_btn);
    lv_obj_add_style(btn2, LV_BTN_PART_MAIN, &style_button_inactive);  // Set initial background
    lv_obj_set_event_cb(btn2, garage_btn_event_cb);
    lv_obj_t *label2 = lv_label_create(btn2, NULL);
    lv_label_set_text(label2, LV_SYMBOL_STOP);
    lv_obj_add_style(label2, LV_LABEL_PART_MAIN, &style_stop_text);  // Use red style
    g_stop_btn = btn2;  // Store button reference

    // Initialize lightbulb color styles
    lv_style_init(&style_lightbulb_on);
    lv_style_set_text_font(&style_lightbulb_on, LV_STATE_DEFAULT, &lv_font_fontawesome_32);
    lv_style_set_text_color(&style_lightbulb_on, LV_STATE_DEFAULT, lv_color_hex(0xFFEB3B));  // Light yellow
    
    lv_style_init(&style_lightbulb_off);
    lv_style_set_text_font(&style_lightbulb_off, LV_STATE_DEFAULT, &lv_font_fontawesome_32);
    lv_style_set_text_color(&style_lightbulb_off, LV_STATE_DEFAULT, lv_color_hex(0x9E9E9E));  // Gray

    // Button 3: Lightbulb icon - optimized for vertical fit using FontAwesome
    lv_obj_t *btn3 = lv_btn_create(btn_cont, NULL);
    lv_obj_set_size(btn3, 126, 50);
    lv_obj_add_style(btn3, LV_BTN_PART_MAIN, &style_garage_btn);
    lv_obj_set_event_cb(btn3, garage_btn_event_cb);
    lv_obj_t *label3 = lv_label_create(btn3, NULL);
    // Set lightbulb icon using character code
    static char lightbulb_text[8];
    lv_snprintf(lightbulb_text, sizeof(lightbulb_text), "%c%c%c", 0xEF, 0x83, 0xAB);  // UTF-8 encoding for U+F0EB
    lv_label_set_text(label3, lightbulb_text);
    
    // Apply initial style based on lightbulb state
    if (lightbulb_get_state()) {
        lv_obj_add_style(label3, LV_LABEL_PART_MAIN, &style_lightbulb_on);
    } else {
        lv_obj_add_style(label3, LV_LABEL_PART_MAIN, &style_lightbulb_off);
    }
    
    // Store global reference
    g_lightbulb_label = label3;

    // Button 4: Arrow DOWN (LV_SYMBOL_DOWN) - optimized for vertical fit
    lv_obj_t *btn4 = lv_btn_create(btn_cont, NULL);
    lv_obj_set_size(btn4, 126, 50);
    lv_obj_add_style(btn4, LV_BTN_PART_MAIN, &style_garage_btn);
    lv_obj_add_style(btn4, LV_BTN_PART_MAIN, &style_button_inactive);  // Set initial background
    lv_obj_set_event_cb(btn4, garage_btn_event_cb);
    lv_obj_t *label4 = lv_label_create(btn4, NULL);
    lv_label_set_text(label4, LV_SYMBOL_DOWN);
    lv_obj_add_style(label4, LV_LABEL_PART_MAIN, &style_button_text);
    g_down_btn = btn4;  // Store button reference

    // Create RIGHT panel - "data" (50% width) - full height
    lv_obj_t *data_panel = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_set_size(data_panel, (LV_HOR_RES / 2 - 5) * 0.9, LV_VER_RES);  // 10% narrower, full height
    lv_obj_set_pos(data_panel, LV_HOR_RES / 2 + 5, 0);
    lv_cont_set_layout(data_panel, LV_LAYOUT_COLUMN_LEFT);

    // Add padding and reduce spacing between rows
    static lv_style_t style_data_panel;
    lv_style_init(&style_data_panel);
    lv_style_set_bg_opa(&style_data_panel, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_style_set_border_width(&style_data_panel, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_top(&style_data_panel, LV_STATE_DEFAULT, 40);      // 40px top padding (was 50px)
    lv_style_set_pad_bottom(&style_data_panel, LV_STATE_DEFAULT, 50);   // 50px bottom padding
    lv_style_set_pad_left(&style_data_panel, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&style_data_panel, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_inner(&style_data_panel, LV_STATE_DEFAULT, 3);
    lv_obj_add_style(data_panel, LV_CONT_PART_MAIN, &style_data_panel);

    row_height = (LV_VER_RES / 5) * 0.75;  // 25% smaller rows (was 20%)
    panel_width = (LV_HOR_RES / 2 - 5) * 0.9;

    // Row 1: brightness slider (full width, no label)
    lv_obj_t *row1 = lv_cont_create(data_panel, NULL);
    lv_obj_set_size(row1, panel_width, row_height);
    lv_cont_set_layout(row1, LV_LAYOUT_OFF);
    lv_obj_add_style(row1, LV_CONT_PART_MAIN, &style_transparent);

    lv_obj_t *bright_slider = lv_slider_create(row1, NULL);
    lv_slider_set_value(bright_slider, 120, LV_ANIM_OFF);
    lv_slider_set_range(bright_slider, 0, 255);
    lv_obj_set_size(bright_slider, panel_width - 10, 10);  // Full width minus margins
    lv_obj_align(bright_slider, row1, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_event_cb(bright_slider, brightness_slider_event_cb);

    // Row 2: outdoor temperature
    lv_obj_t *row2 = lv_cont_create(data_panel, NULL);
    lv_obj_set_size(row2, panel_width, row_height);
    lv_cont_set_layout(row2, LV_LAYOUT_OFF);
    lv_obj_add_style(row2, LV_CONT_PART_MAIN, &style_transparent);

    lv_obj_t *outdoor_label = lv_label_create(row2, NULL);
    lv_label_set_text(outdoor_label, "outdoor");
    lv_obj_align(outdoor_label, row2, LV_ALIGN_IN_LEFT_MID, 5, 0);

    // Create temperature value label with degree symbol (positioned at right edge)
    static lv_style_t style_temp_value;
    lv_style_init(&style_temp_value);
    lv_style_set_text_font(&style_temp_value, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    
    lv_obj_t *outdoor_value = lv_label_create(row2, NULL);
    lv_label_set_text(outdoor_value, "-");  // Default to "-" until MQTT data received
    lv_obj_add_style(outdoor_value, LV_LABEL_PART_MAIN, &style_temp_value);
    lv_obj_align(outdoor_value, row2, LV_ALIGN_IN_RIGHT_MID, -40, 0);  // 15px more to the left (total 40px from edge)
    g_outdoor_temp_label = outdoor_value;  // Store reference
    
    // Remove the separate degree label
    lv_obj_t *outdoor_degree = NULL;  // Keep variable for compatibility but unused

    // Row 3: indoor temperature
    lv_obj_t *row3 = lv_cont_create(data_panel, NULL);
    lv_obj_set_size(row3, panel_width, row_height);
    lv_cont_set_layout(row3, LV_LAYOUT_OFF);
    lv_obj_add_style(row3, LV_CONT_PART_MAIN, &style_transparent);

    lv_obj_t *indoor_label = lv_label_create(row3, NULL);
    lv_label_set_text(indoor_label, "indoor");
    lv_obj_align(indoor_label, row3, LV_ALIGN_IN_LEFT_MID, 5, 0);

    // Create temperature value label with degree symbol (positioned at right edge)
    lv_obj_t *indoor_value = lv_label_create(row3, NULL);
    lv_label_set_text(indoor_value, "-");  // Default to "-" until MQTT data received
    lv_obj_add_style(indoor_value, LV_LABEL_PART_MAIN, &style_temp_value);
    lv_obj_align(indoor_value, row3, LV_ALIGN_IN_RIGHT_MID, -40, 0);  // 15px more to the left (total 40px from edge)
    g_indoor_temp_label = indoor_value;  // Store reference
    
    // Remove the separate degree label
    lv_obj_t *indoor_degree = NULL;  // Keep variable for compatibility but unused

    // Row 4: entrance switch
    lv_obj_t *row4 = lv_cont_create(data_panel, NULL);
    lv_obj_set_size(row4, panel_width, row_height);
    lv_cont_set_layout(row4, LV_LAYOUT_OFF);
    lv_obj_add_style(row4, LV_CONT_PART_MAIN, &style_transparent);

    lv_obj_t *entrance_label = lv_label_create(row4, NULL);
    lv_label_set_text(entrance_label, "entrance");
    lv_obj_align(entrance_label, row4, LV_ALIGN_IN_LEFT_MID, 5, 0);

    lv_obj_t *entrance_switch = lv_switch_create(row4, NULL);
    lv_obj_set_size(entrance_switch, 50, 25);
    lv_obj_align(entrance_switch, row4, LV_ALIGN_IN_RIGHT_MID, -5, 0);
    lv_obj_set_event_cb(entrance_switch, entrance_switch_event_cb);  // Add event callback
    g_entrance_switch = entrance_switch;  // Store reference

    // Row 5: cat-door switch
    lv_obj_t *row5 = lv_cont_create(data_panel, NULL);
    lv_obj_set_size(row5, panel_width, row_height);
    lv_cont_set_layout(row5, LV_LAYOUT_OFF);
    lv_obj_add_style(row5, LV_CONT_PART_MAIN, &style_transparent);

    lv_obj_t *catdoor_label = lv_label_create(row5, NULL);
    lv_label_set_text(catdoor_label, "cat-door");
    lv_obj_align(catdoor_label, row5, LV_ALIGN_IN_LEFT_MID, 5, 0);

    lv_obj_t *catdoor_switch = lv_switch_create(row5, NULL);
    lv_obj_set_size(catdoor_switch, 50, 25);
    lv_obj_align(catdoor_switch, row5, LV_ALIGN_IN_RIGHT_MID, -5, 0);
    lv_obj_set_event_cb(catdoor_switch, catdoor_switch_event_cb);  // Add event callback
    g_catdoor_switch = catdoor_switch;  // Store reference
}

// Exported UI control functions for MQTT handlers

extern "C" void set_up_button_moving(bool moving)
{
    // TODO: Implement blinking light yellow background when moving
    // For now, just set it to active state
    if (g_up_btn) {
        update_button_background(g_up_btn, moving);
    }
}

extern "C" void set_down_button_moving(bool moving)
{
    // TODO: Implement blinking light yellow background when moving
    // For now, just set it to active state
    if (g_down_btn) {
        update_button_background(g_down_btn, moving);
    }
}

extern "C" void set_lightbulb_active(bool active)
{
    if (g_lightbulb_label) {
        // Update internal state
        if (active) {
            lightbulb_on();
        } else {
            lightbulb_off();
        }
        // Update UI color
        update_lightbulb_color(g_lightbulb_label, active);
    }
}

extern "C" void set_entrance_switch_state(bool state)
{
    if (g_entrance_switch) {
        if (state) {
            lv_switch_on(g_entrance_switch, LV_ANIM_OFF);
        } else {
            lv_switch_off(g_entrance_switch, LV_ANIM_OFF);
        }
    }
}

static void entrance_switch_event_cb(lv_obj_t *sw, lv_event_t e)
{
    if (e == LV_EVENT_VALUE_CHANGED) {
        bool is_on = lv_switch_get_state(sw);
        const char* command = is_on ? "on" : "off";
        
        // Publish MQTT command
        mqtt_publish_command("entrance/relay", command);
    }
}

static void catdoor_switch_event_cb(lv_obj_t *sw, lv_event_t e)
{
    if (e == LV_EVENT_VALUE_CHANGED) {
        bool is_on = lv_switch_get_state(sw);
        const char* command = is_on ? "on" : "off";
        
        // Publish MQTT command
        mqtt_publish_command("cat-door/relay", command);
    }
}

extern "C" void set_catdoor_switch_state(bool state)
{
    if (g_catdoor_switch) {
        if (state) {
            lv_switch_on(g_catdoor_switch, LV_ANIM_OFF);
        } else {
            lv_switch_off(g_catdoor_switch, LV_ANIM_OFF);
        }
    }
}

extern "C" void set_outdoor_temperature(float temperature)
{
    if (g_outdoor_temp_label) {
        static char temp_str[16];
        snprintf(temp_str, sizeof(temp_str), "%.1f°", temperature);
        lv_label_set_text(g_outdoor_temp_label, temp_str);
    }
}

extern "C" void set_indoor_temperature(float temperature)
{
    if (g_indoor_temp_label) {
        static char temp_str[16];
        snprintf(temp_str, sizeof(temp_str), "%.1f°", temperature);
        lv_label_set_text(g_indoor_temp_label, temp_str);
    }
}

static void brightness_slider_event_cb(lv_obj_t *slider, lv_event_t e)
{
    if (e == LV_EVENT_VALUE_CHANGED) {
        int16_t val = lv_slider_get_value(slider);
        setBrightness(val);
    }
}

static void update_lightbulb_color(lv_obj_t *label, bool is_on)
{
    if (label == NULL) {
        return;
    }
    
    // Create a local style to modify the text color
    static lv_style_t style_local;
    lv_style_init(&style_local);
    lv_style_set_text_font(&style_local, LV_STATE_DEFAULT, &lv_font_fontawesome_32);
    
    // Set color based on state
    if (is_on) {
        lv_style_set_text_color(&style_local, LV_STATE_DEFAULT, lv_color_hex(0xFFEB3B));  // Light yellow
    } else {
        lv_style_set_text_color(&style_local, LV_STATE_DEFAULT, lv_color_hex(0x9E9E9E));  // Gray
    }
    
    // Apply the style
    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &style_local);
}

static void update_button_background(lv_obj_t *btn, bool is_active)
{
    if (btn == NULL) {
        return;
    }
    
    // Apply the appropriate background style
    if (is_active) {
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_button_active);
    } else {
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_button_inactive);
    }
}

static void update_stop_button_state(void)
{
    update_button_background(g_stop_btn, g_stop_active);
}

static void update_up_down_buttons(lv_obj_t *active_btn)
{
    // If active_btn is NULL, both should be inactive
    // Otherwise, the active one gets highlighted and the other becomes inactive
    if (active_btn == NULL) {
        update_button_background(g_up_btn, false);
        update_button_background(g_down_btn, false);
    } else if (active_btn == g_up_btn) {
        update_button_background(g_up_btn, true);
        update_button_background(g_down_btn, false);
    } else if (active_btn == g_down_btn) {
        update_button_background(g_up_btn, false);
        update_button_background(g_down_btn, true);
    }
}

static void garage_btn_event_cb(lv_obj_t *obj, lv_event_t e)
{
    if (e == LV_EVENT_CLICKED) {
        // Get button label to determine which button was clicked
        lv_obj_t *label = lv_obj_get_child(obj, NULL);
        
        // Check if this is the lightbulb button by comparing the label pointer
        if (label == g_lightbulb_label) {
            // Lightbulb icon pressed - send MQTT command to toggle
            bool current_state = lightbulb_get_state();
            const char* command = current_state ? "OFF" : "ON";
            
            // Publish MQTT command
            mqtt_publish_command("hormann/garage-door/command/lamp", command);
            
            // Update local state and UI immediately for responsiveness
            if (current_state) {
                lightbulb_off();
            } else {
                lightbulb_on();
            }
            update_lightbulb_color(g_lightbulb_label, lightbulb_get_state());
            
        } else if (obj == g_stop_btn) {
            // STOP button - send MQTT command to stop door
            mqtt_publish_command("hormann/garage-door/command/door", "stop");
            
            // Update local state
            g_stop_active = !g_stop_active;
            update_stop_button_state();
            
            if (g_stop_active) {
                // Deactivate UP/DOWN when STOP is active and reset their state flags
                update_up_down_buttons(NULL);
                g_up_active = false;
                g_down_active = false;
                relayTurnOff();
            }
            
        } else if (obj == g_up_btn) {
            // UP button pressed - send MQTT command to open door
            mqtt_publish_command("hormann/garage-door/command/door", "open");
            
            // Update local state
            if (g_up_active) {
                // Clicking active UP button deactivates it
                update_up_down_buttons(NULL);
                g_up_active = false;
            } else {
                // Activate UP, deactivate DOWN
                update_up_down_buttons(g_up_btn);
                g_up_active = true;
                g_down_active = false;
                relayTurnOn();
                
                // Deactivate STOP if it was active
                if (g_stop_active) {
                    g_stop_active = false;
                    update_stop_button_state();
                }
            }
            
        } else if (obj == g_down_btn) {
            // DOWN button pressed - send MQTT command to close door
            mqtt_publish_command("hormann/garage-door/command/door", "close");
            
            // Update local state
            if (g_down_active) {
                // Clicking active DOWN button deactivates it
                update_up_down_buttons(NULL);
                g_down_active = false;
            } else {
                // Activate DOWN, deactivate UP
                update_up_down_buttons(g_down_btn);
                g_down_active = true;
                g_up_active = false;
                relayTurnOff();
                
                // Deactivate STOP if it was active
                if (g_stop_active) {
                    g_stop_active = false;
                    update_stop_button_state();
                }
            }
        }
    }
}
