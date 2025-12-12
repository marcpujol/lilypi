#include "config.h"
#include "lv_demo_widgets.h"

TTGOClass *ttgo;

void relayTurnOn(void)
{
    ttgo->turnOnRelay();
}

void relayTurnOff(void)
{
    ttgo->turnOffRelay();
}

void setBrightness(uint8_t level)
{
    ttgo->setBrightness(level);
}

void turnOnUSB()
{
    ttgo->turnOnUSB();
}

void turnOffUSB()
{
    ttgo->turnOffUSB();
}

float getVoltage()
{
    return ttgo->getVoltage();
}

void setup()
{
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n=== LilyPi V1 - LVGL Demo (Landscape) ===");

    // Get watch instance
    ttgo = TTGOClass::getWatch();
    Serial.println("✓ TTGOClass initialized");

    // Initialize the hardware
    ttgo->begin();
    Serial.println("✓ Hardware initialized");

    // Turn on the backlight
    ttgo->openBL();
    Serial.println("✓ Backlight on");

    // Set landscape orientation BEFORE lvgl_begin
    ttgo->tft->setRotation(1);
    Serial.println("✓ Rotation set to landscape (before LVGL)");

    // Initialize LVGL using the library's method
    // This will detect the current rotation and configure accordingly
    ttgo->lvgl_begin();
    Serial.println("✓ LVGL initialized in landscape mode");

    // Check SD card
    if (!ttgo->sdcard_begin()) {
        Serial.println("⚠ SD card: NOT FOUND");
    } else {
        Serial.println("✓ SD card mounted");
    }

    // Check RTC
    if (!ttgo->deviceProbe(0x51)) {
        Serial.println("⚠ RTC CHECK FAILED");
    } else {
        Serial.println("✓ RTC detected");
    }

    // Start LVGL demo
    Serial.println("✓ Starting LVGL demo widgets...");
    lv_demo_widgets();
    
    Serial.println("\n✓✓✓ ALL READY - Full screen landscape! ✓✓✓\n");
}

void loop()
{
    lv_task_handler();
    delay(5);
}
