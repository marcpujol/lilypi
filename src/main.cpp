#include "config.h"
#include "lv_demo_widgets.h"
#include "wifi_manager.h"
#include "wifi_screen.h"
#include "mqtt_manager.h"
#include <ArduinoJson.h>

extern "C" {
    #include "../fonts/lightbulb.h"
}

TTGOClass *ttgo;
WiFiManager wifiManager;
MQTTManager mqttManager;
bool wifi_connected = false;
bool mqtt_connected = false;
bool main_ui_loaded = false;

// Forward declarations for relay/utility functions
void relayTurnOn(void);
void relayTurnOff(void);
void setBrightness(uint8_t level);

// Forward declarations for UI control functions
extern "C" {
    void set_up_button_moving(bool moving);
    void set_down_button_moving(bool moving);
    void set_lightbulb_active(bool active);
    void set_entrance_switch_state(bool state);
    void set_catdoor_switch_state(bool state);
    void set_outdoor_temperature(float temperature);
    void set_indoor_temperature(float temperature);
}

// Forward declarations for topic handlers
void handleGarageDoorState(const char* payload, unsigned int length);
void handleMeteoTemperature(const char* payload, unsigned int length);
void handleShedTemperature(const char* payload, unsigned int length);
void handleEntranceRelayState(const char* payload, unsigned int length);
void handleCatDoorRelayState(const char* payload, unsigned int length);

// MQTT callback function - routes messages to specific handlers
void mqttCallback(char* topic, uint8_t* payload, unsigned int length) {
    Serial.print("MQTT message received on topic: ");
    Serial.println(topic);
    
    // Convert payload to null-terminated string
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';
    
    Serial.print("Payload: ");
    Serial.println(message);
    
    // Route to appropriate handler based on topic
    if (strcmp(topic, "hormann/garage-door/state") == 0) {
        handleGarageDoorState(message, length);
    } else if (strcmp(topic, "meteo/temperature") == 0) {
        handleMeteoTemperature(message, length);
    } else if (strcmp(topic, "shed/temperature") == 0) {
        handleShedTemperature(message, length);
    } else if (strcmp(topic, "entrance/relay/state") == 0) {
        handleEntranceRelayState(message, length);
    } else if (strcmp(topic, "cat-door/relay/state") == 0) {
        handleCatDoorRelayState(message, length);
    } else {
        Serial.print("⚠ Unknown topic: ");
        Serial.println(topic);
    }
}

// Handler for hormann/garage-door/state topic
// Receives JSON: {"valid":true,"doorposition":0,"lamp":"false","doorstate":"closed","detailedState":"closed","vent":"close","half":"close"}
void handleGarageDoorState(const char* payload, unsigned int length) {
    Serial.println("→ Parsing Hörmann garage door state JSON");
    
    // Parse JSON
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, payload, length);
    
    if (error) {
        Serial.print("  ❌ JSON parse error: ");
        Serial.println(error.c_str());
        return;
    }
    
    // Extract fields
    const char* doorstate = doc["doorstate"] | "unknown";
    
    // Handle lamp field - can be boolean or string
    bool lamp = false;
    if (doc["lamp"].is<bool>()) {
        lamp = doc["lamp"];
    } else if (doc["lamp"].is<const char*>()) {
        const char* lamp_str = doc["lamp"];
        lamp = (strcmp(lamp_str, "true") == 0);
    }
    
    Serial.print("  Door state: ");
    Serial.println(doorstate);
    Serial.print("  Lamp: ");
    Serial.println(lamp ? "ON" : "OFF");
    
    // Update lightbulb based on lamp state
    set_lightbulb_active(lamp);
    
    // Update UI based on door state
    if (strcmp(doorstate, "opening") == 0) {
        Serial.println("  → Door is OPENING - Setting UP button to moving state");
        set_up_button_moving(true);
        set_down_button_moving(false);
    } else if (strcmp(doorstate, "closing") == 0) {
        Serial.println("  → Door is CLOSING - Setting DOWN button to moving state");
        set_up_button_moving(false);
        set_down_button_moving(true);
    } else if (strcmp(doorstate, "open") == 0) {
        Serial.println("  → Door is OPEN");
        set_up_button_moving(false);
        set_down_button_moving(false);
    } else if (strcmp(doorstate, "closed") == 0) {
        Serial.println("  → Door is CLOSED");
        set_up_button_moving(false);
        set_down_button_moving(false);
    } else if (strcmp(doorstate, "stopped") == 0) {
        Serial.println("  → Door is STOPPED");
        set_up_button_moving(false);
        set_down_button_moving(false);
    }
}

// Handler for meteo/temperature topic
// Receives outdoor temperature value in format "XX.X"
void handleMeteoTemperature(const char* payload, unsigned int length) {
    float temperature = atof(payload);
    Serial.print("→ Meteo temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    
    // Update UI outdoor temperature label
    set_outdoor_temperature(temperature);
}

// Handler for shed/temperature topic
// Receives shed/indoor temperature value in format "XX.X"
void handleShedTemperature(const char* payload, unsigned int length) {
    float temperature = atof(payload);
    Serial.print("→ Shed temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    
    // Update UI indoor temperature label
    set_indoor_temperature(temperature);
}

// Handler for entrance/relay/state topic
// Receives entrance relay state: "on" or "off"
void handleEntranceRelayState(const char* payload, unsigned int length) {
    Serial.print("→ Entrance relay state: ");
    Serial.println(payload);
    
    if (strcasecmp(payload, "on") == 0) {
        Serial.println("  Setting entrance switch to ON");
        set_entrance_switch_state(true);
    } else if (strcasecmp(payload, "off") == 0) {
        Serial.println("  Setting entrance switch to OFF");
        set_entrance_switch_state(false);
    }
}

// Handler for cat-door/relay/state topic
// Receives cat door relay state: "on" or "off"
void handleCatDoorRelayState(const char* payload, unsigned int length) {
    Serial.print("→ Cat door relay state: ");
    Serial.println(payload);
    
    if (strcasecmp(payload, "on") == 0) {
        Serial.println("  Setting cat-door switch to ON");
        set_catdoor_switch_state(true);
    } else if (strcasecmp(payload, "off") == 0) {
        Serial.println("  Setting cat-door switch to OFF");
        set_catdoor_switch_state(false);
    }
}

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

// MQTT publish command function for UI buttons
extern "C" bool mqtt_publish_command(const char* topic, const char* payload)
{
    if (!mqtt_connected) {
        Serial.println("⚠ MQTT not connected, cannot publish command");
        return false;
    }
    
    Serial.print("Publishing MQTT command: ");
    Serial.print(topic);
    Serial.print(" -> ");
    Serial.println(payload);
    
    return mqttManager.publish(topic, payload, false);
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
    
    Serial.println("\n=== LilyPi V1 - LVGL Demo (Landscape) with WiFi ===");

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

    // Show WiFi connection screen FIRST
    Serial.println("✓ Creating WiFi connection screen...");
    wifi_screen_create();
    
    // Force screen to render immediately
    lv_task_handler();
    delay(100);
    lv_task_handler();

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

    // Initialize WiFi
    Serial.println("\n--- WiFi Configuration ---");
    wifiManager.begin(WIFI_SSID, WIFI_PASSWORD);
    
    Serial.println("\n✓✓✓ Setup complete - WiFi connection in progress ✓✓✓\n");
}

void loop()
{
    // If WiFi not connected yet, try to connect
    if (!wifi_connected) {
        wifi_screen_update_status("Connecting to WiFi...");
        
        if (wifiManager.connect(1, 5000)) {
            Serial.print("✓ WiFi connected! IP: ");
            Serial.println(wifiManager.getIPAddress());
            wifi_connected = true;
            wifi_screen_update_status("WiFi Connected!");
            delay(1000); // Show "Connected!" message briefly
        } else {
            // Keep trying in the loop
            delay(2000);
        }
    }
    
    // Once WiFi is connected, initialize MQTT
    if (wifi_connected && !mqtt_connected) {
        Serial.println("\n--- MQTT Configuration ---");
        mqttManager.begin(MQTT_SERVER, MQTT_PORT, MQTT_CLIENT_ID);
        mqttManager.setCallback(mqttCallback);
        
        if (mqttManager.connect()) {
            mqtt_connected = true;
            
            // Subscribe to topics
            const char* topics[] = {
                "hormann/garage-door/state",
                "meteo/temperature",
                "shed/temperature",
                "entrance/relay/state",
                "cat-door/relay/state"
            };
            
            mqttManager.subscribeMultiple(topics, 5);
            Serial.println("✓ MQTT initialized and subscribed to topics\n");
        } else {
            Serial.println("⚠ MQTT connection failed, will retry...\n");
            delay(5000);  // Wait before retrying
        }
    }
    
    // Once WiFi and MQTT are connected, load main UI
    if (wifi_connected && mqtt_connected && !main_ui_loaded) {
        Serial.println("✓ Hiding WiFi screen and loading main UI...");
        wifi_screen_hide();
        
        Serial.println("✓ Starting LVGL demo widgets...");
        lv_demo_widgets();
        
        main_ui_loaded = true;
        Serial.println("\n✓✓✓ ALL READY - Full screen landscape with WiFi and MQTT! ✓✓✓\n");
    }
    
    // Monitor WiFi status in background
    if (wifi_connected) {
        wifiManager.monitor();
    }
    
    // Handle MQTT messages
    if (mqtt_connected) {
        mqttManager.loop();
    }
    
    // Handle LVGL display refresh
    lv_task_handler();
    
    delay(5);
}
