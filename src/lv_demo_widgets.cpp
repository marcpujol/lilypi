#include "lvgl/lvgl.h"
#include <stdio.h>

// Declare FontAwesome font
LV_FONT_DECLARE(lv_font_fontawesome_16);

extern void setBrightness(uint8_t level);
extern void relayTurnOff(void);
extern void relayTurnOn(void);
extern void turnOnUSB(void);
extern void turnOffUSB(void);

static void brightness_slider_event_cb(lv_obj_t *slider, lv_event_t e);
static void garage_btn_event_cb(lv_obj_t *obj, lv_event_t e);

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
    
    // Style for garage buttons with rounded borders
    static lv_style_t style_garage_btn;
    lv_style_init(&style_garage_btn);
    lv_style_set_radius(&style_garage_btn, LV_STATE_DEFAULT, 15);
    lv_style_set_border_width(&style_garage_btn, LV_STATE_DEFAULT, 2);
    lv_style_set_border_color(&style_garage_btn, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_pad_all(&style_garage_btn, LV_STATE_DEFAULT, 20);

    // Create container for vertically stacked buttons
    lv_obj_t *btn_cont = lv_cont_create(garage_panel, NULL);
    lv_cont_set_layout(btn_cont, LV_LAYOUT_COLUMN_MID);
    lv_cont_set_fit(btn_cont, LV_FIT_TIGHT);
    lv_obj_add_style(btn_cont, LV_CONT_PART_MAIN, &style_transparent);

    // Button 1: Arrow UP (LV_SYMBOL_UP) - optimized for vertical fit
    lv_obj_t *btn1 = lv_btn_create(btn_cont, NULL);
    lv_obj_set_size(btn1, 126, 50);
    lv_obj_add_style(btn1, LV_BTN_PART_MAIN, &style_garage_btn);
    lv_obj_set_event_cb(btn1, garage_btn_event_cb);
    lv_obj_t *label1 = lv_label_create(btn1, NULL);
    lv_label_set_text(label1, LV_SYMBOL_UP);
    static lv_style_t style_icon;
    lv_style_init(&style_icon);
    lv_style_set_text_font(&style_icon, LV_STATE_DEFAULT, &lv_font_montserrat_32);
    lv_obj_add_style(label1, LV_LABEL_PART_MAIN, &style_icon);

    // Button 2: STOP (LV_SYMBOL_STOP) - optimized for vertical fit
    lv_obj_t *btn2 = lv_btn_create(btn_cont, NULL);
    lv_obj_set_size(btn2, 126, 50);
    lv_obj_add_style(btn2, LV_BTN_PART_MAIN, &style_garage_btn);
    lv_obj_set_event_cb(btn2, garage_btn_event_cb);
    lv_obj_t *label2 = lv_label_create(btn2, NULL);
    lv_label_set_text(label2, LV_SYMBOL_STOP);
    lv_obj_add_style(label2, LV_LABEL_PART_MAIN, &style_icon);

    // Button 3: Power icon - optimized for vertical fit using FontAwesome
    lv_obj_t *btn3 = lv_btn_create(btn_cont, NULL);
    lv_obj_set_size(btn3, 126, 50);
    lv_obj_add_style(btn3, LV_BTN_PART_MAIN, &style_garage_btn);
    lv_obj_set_event_cb(btn3, garage_btn_event_cb);
    lv_obj_t *label3 = lv_label_create(btn3, NULL);
    
    // Use FontAwesome font for power icon
    static lv_style_t style_fontawesome;
    lv_style_init(&style_fontawesome);
    lv_style_set_text_font(&style_fontawesome, LV_STATE_DEFAULT, &lv_font_fontawesome_16);
    lv_obj_add_style(label3, LV_LABEL_PART_MAIN, &style_fontawesome);
    
    lv_label_set_text(label3, "");  // UTF-8 for FontAwesome power icon (U+F011)

    // Button 4: Arrow DOWN (LV_SYMBOL_DOWN) - optimized for vertical fit
    lv_obj_t *btn4 = lv_btn_create(btn_cont, NULL);
    lv_obj_set_size(btn4, 126, 50);
    lv_obj_add_style(btn4, LV_BTN_PART_MAIN, &style_garage_btn);
    lv_obj_set_event_cb(btn4, garage_btn_event_cb);
    lv_obj_t *label4 = lv_label_create(btn4, NULL);
    lv_label_set_text(label4, LV_SYMBOL_DOWN);
    lv_obj_add_style(label4, LV_LABEL_PART_MAIN, &style_icon);

    // Create RIGHT panel - "data" (50% width) - full height
    lv_obj_t *data_panel = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_set_size(data_panel, (LV_HOR_RES / 2 - 5) * 0.9, LV_VER_RES);  // 10% narrower, full height
    lv_obj_set_pos(data_panel, LV_HOR_RES / 2 + 5, 0);
    lv_cont_set_layout(data_panel, LV_LAYOUT_COLUMN_LEFT);
    
    // Reduce padding between rows
    static lv_style_t style_data_panel;
    lv_style_init(&style_data_panel);
    lv_style_set_bg_opa(&style_data_panel, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_style_set_border_width(&style_data_panel, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_all(&style_data_panel, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_inner(&style_data_panel, LV_STATE_DEFAULT, 3);  // 30% less padding between rows
    lv_obj_add_style(data_panel, LV_CONT_PART_MAIN, &style_data_panel);
    
    row_height = LV_VER_RES / 5;  // Divide vertical space into 5 equal rows (brightness + 4 data rows)
    panel_width = (LV_HOR_RES / 2 - 5) * 0.9;  // Match panel width (10% narrower)
    
    // Row 1: brightness slider
    lv_obj_t *row1 = lv_cont_create(data_panel, NULL);
    lv_obj_set_size(row1, panel_width, row_height);
    lv_cont_set_layout(row1, LV_LAYOUT_OFF);
    lv_obj_add_style(row1, LV_CONT_PART_MAIN, &style_transparent);
    
    lv_obj_t *bright_label = lv_label_create(row1, NULL);
    lv_label_set_text(bright_label, "brightness");
    lv_obj_align(bright_label, row1, LV_ALIGN_IN_LEFT_MID, 5, 0);
    
    lv_obj_t *bright_slider = lv_slider_create(row1, NULL);
    lv_slider_set_value(bright_slider, 120, LV_ANIM_OFF);
    lv_slider_set_range(bright_slider, 0, 255);
    lv_obj_set_size(bright_slider, 80, 10);
    lv_obj_align(bright_slider, row1, LV_ALIGN_IN_RIGHT_MID, -5, 0);
    lv_obj_set_event_cb(bright_slider, brightness_slider_event_cb);
    
    // Row 2: outdoor temperature
    lv_obj_t *row2 = lv_cont_create(data_panel, NULL);
    lv_obj_set_size(row2, panel_width, row_height);
    lv_cont_set_layout(row2, LV_LAYOUT_OFF);
    lv_obj_add_style(row2, LV_CONT_PART_MAIN, &style_transparent);
    
    lv_obj_t *outdoor_label = lv_label_create(row2, NULL);
    lv_label_set_text(outdoor_label, "outdoor");
    lv_obj_align(outdoor_label, row2, LV_ALIGN_IN_LEFT_MID, 5, 0);
    
    lv_obj_t *outdoor_value = lv_label_create(row2, NULL);
    lv_label_set_text(outdoor_value, "20.0º");
    lv_obj_align(outdoor_value, row2, LV_ALIGN_IN_RIGHT_MID, -5, 0);
    
    // Row 3: indoor temperature
    lv_obj_t *row3 = lv_cont_create(data_panel, NULL);
    lv_obj_set_size(row3, panel_width, row_height);
    lv_cont_set_layout(row3, LV_LAYOUT_OFF);
    lv_obj_add_style(row3, LV_CONT_PART_MAIN, &style_transparent);
    
    lv_obj_t *indoor_label = lv_label_create(row3, NULL);
    lv_label_set_text(indoor_label, "indoor");
    lv_obj_align(indoor_label, row3, LV_ALIGN_IN_LEFT_MID, 5, 0);
    
    lv_obj_t *indoor_value = lv_label_create(row3, NULL);
    lv_label_set_text(indoor_value, "20.0º");
    lv_obj_align(indoor_value, row3, LV_ALIGN_IN_RIGHT_MID, -5, 0);
    
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
}

static void brightness_slider_event_cb(lv_obj_t *slider, lv_event_t e)
{
    if (e == LV_EVENT_VALUE_CHANGED) {
        int16_t val = lv_slider_get_value(slider);
        setBrightness(val);
    }
}

static void garage_btn_event_cb(lv_obj_t *obj, lv_event_t e)
{
    if (e == LV_EVENT_CLICKED) {
        // Get button label to determine which button was clicked
        lv_obj_t *label = lv_obj_get_child(obj, NULL);
        const char *text = lv_label_get_text(label);
        
        // Handle button actions based on symbol
        if (strcmp(text, LV_SYMBOL_UP) == 0) {
            // Arrow UP pressed - open garage
            relayTurnOn();
        } else if (strcmp(text, LV_SYMBOL_STOP) == 0) {
            // STOP pressed - stop garage movement
            relayTurnOff();
        } else if (strcmp(text, "") == 0) {
            // Power icon pressed - toggle light (using power icon 0xf011)
            turnOnUSB();
        } else if (strcmp(text, LV_SYMBOL_DOWN) == 0) {
            // Arrow DOWN pressed - close garage
            relayTurnOff();
        }
    }
}
