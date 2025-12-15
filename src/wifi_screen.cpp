#include "wifi_screen.h"
#include "lvgl/lvgl.h"

// FontAwesome WiFi icon constant
#define FA_WIFI "\xEF\x87\xAB"  // U+F1EB

// Declare FontAwesome font
LV_FONT_DECLARE(lv_font_fontawesome_16);

// Declare Montserrat fonts (16 is ~25% smaller than 22)
LV_FONT_DECLARE(lv_font_montserrat_16);

// WiFi screen objects
static lv_obj_t *wifi_screen = NULL;
static lv_obj_t *wifi_icon = NULL;
static lv_obj_t *status_label = NULL;
static lv_task_t *blink_task = NULL;
static bool icon_visible = true;

// Blink task callback
static void wifi_icon_blink_task(lv_task_t *task)
{
    (void)task;  // Unused parameter
    
    if (wifi_icon == NULL) {
        return;
    }
    
    if (icon_visible) {
        lv_obj_set_hidden(wifi_icon, true);
        icon_visible = false;
    } else {
        lv_obj_set_hidden(wifi_icon, false);
        icon_visible = true;
    }
}

void wifi_screen_create(void)
{
    // Create a full-screen container
    wifi_screen = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(wifi_screen, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(wifi_screen, 0, 0);
    
    // Set dark gray background
    static lv_style_t style_screen;
    lv_style_init(&style_screen);
    lv_style_set_bg_color(&style_screen, LV_STATE_DEFAULT, lv_color_hex(0x2A2A2A));
    lv_style_set_border_width(&style_screen, LV_STATE_DEFAULT, 0);
    lv_obj_add_style(wifi_screen, LV_OBJ_PART_MAIN, &style_screen);
    
    // Create container for centered content
    lv_obj_t *content = lv_cont_create(wifi_screen, NULL);
    lv_cont_set_layout(content, LV_LAYOUT_COLUMN_MID);
    lv_obj_set_size(content, LV_HOR_RES, LV_VER_RES);
    lv_obj_align(content, wifi_screen, LV_ALIGN_CENTER, 0, 0);
    
    // Make container transparent
    static lv_style_t style_transparent;
    lv_style_init(&style_transparent);
    lv_style_set_bg_opa(&style_transparent, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_style_set_border_width(&style_transparent, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_inner(&style_transparent, LV_STATE_DEFAULT, 20);
    lv_obj_add_style(content, LV_CONT_PART_MAIN, &style_transparent);
    
    // Create WiFi icon label using FontAwesome
    wifi_icon = lv_label_create(content, NULL);
    lv_label_set_text(wifi_icon, FA_WIFI);  // FontAwesome WiFi icon (U+F1EB)
    
    // Style for WiFi icon (light gray, blinking, with 60px top padding)
    static lv_style_t style_icon;
    lv_style_init(&style_icon);
    lv_style_set_text_font(&style_icon, LV_STATE_DEFAULT, &lv_font_fontawesome_16);
    lv_style_set_text_color(&style_icon, LV_STATE_DEFAULT, lv_color_hex(0xD0D0D0));
    lv_style_set_pad_top(&style_icon, LV_STATE_DEFAULT, 60);
    lv_obj_add_style(wifi_icon, LV_LABEL_PART_MAIN, &style_icon);
    
    // Create status message label
    status_label = lv_label_create(content, NULL);
    lv_label_set_text(status_label, "Connecting to WiFi...");
    lv_label_set_long_mode(status_label, LV_LABEL_LONG_BREAK);
    lv_label_set_align(status_label, LV_LABEL_ALIGN_CENTER);
    lv_obj_set_width(status_label, LV_HOR_RES - 40);
    
    // Style for status label (very light gray, Montserrat 16 - 25% smaller than 22)
    static lv_style_t style_status;
    lv_style_init(&style_status);
    lv_style_set_text_color(&style_status, LV_STATE_DEFAULT, lv_color_hex(0xD0D0D0));
    lv_style_set_text_font(&style_status, LV_STATE_DEFAULT, &lv_font_montserrat_16);
    lv_obj_add_style(status_label, LV_LABEL_PART_MAIN, &style_status);
    
    // Create blink task (500ms interval)
    blink_task = lv_task_create(wifi_icon_blink_task, 500, LV_TASK_PRIO_MID, NULL);
    
    // Bring WiFi screen to front
    lv_obj_move_foreground(wifi_screen);
}

void wifi_screen_hide(void)
{
    if (blink_task != NULL) {
        lv_task_del(blink_task);
        blink_task = NULL;
    }
    
    if (wifi_screen != NULL) {
        lv_obj_del(wifi_screen);
        wifi_screen = NULL;
        wifi_icon = NULL;
        status_label = NULL;
    }
}

void wifi_screen_update_status(const char* message)
{
    if (status_label != NULL) {
        lv_label_set_text(status_label, message);
    }
}
