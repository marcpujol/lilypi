#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>

// WiFi status callback type
typedef void (*wifi_status_callback_t)(bool connected, const char* message);

class WiFiManager {
public:
    WiFiManager();
    
    // Initialize WiFi with credentials
    void begin(const char* ssid, const char* password);
    
    // Blocking connection with retry logic
    bool connect(uint8_t max_attempts = 3, uint32_t timeout_ms = 20000);
    
    // Check if connected
    bool isConnected();
    
    // Get IP address as string
    String getIPAddress();
    
    // Background monitoring
    void monitor();
    
    // Set status callback
    void setStatusCallback(wifi_status_callback_t callback);
    
    // Disconnect
    void disconnect();

private:
    const char* _ssid;
    const char* _password;
    wifi_status_callback_t _status_callback;
    bool _was_connected;
    unsigned long _last_check;
    uint32_t _check_interval;
    
    void notifyStatus(bool connected, const char* message);
};

#endif
