#include "wifi_manager.h"
#include "config.h"

WiFiManager::WiFiManager() : 
    _ssid(nullptr),
    _password(nullptr),
    _status_callback(nullptr),
    _was_connected(false),
    _last_check(0),
    _check_interval(5000) {
}

void WiFiManager::begin(const char* ssid, const char* password) {
    _ssid = ssid;
    _password = password;
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
}

bool WiFiManager::connect(uint8_t max_attempts, uint32_t timeout_ms) {
    Serial.printf("\n[WiFi] Connecting to '%s'...\n", _ssid);
    
    for (uint8_t attempt = 1; attempt <= max_attempts; attempt++) {
        Serial.printf("[WiFi] Attempt %d/%d\n", attempt, max_attempts);
        notifyStatus(false, "Connecting...");
        
        WiFi.begin(_ssid, _password);
        
        unsigned long start = millis();
        while (WiFi.status() != WL_CONNECTED && (millis() - start) < timeout_ms) {
            delay(100);
        }
        
        if (WiFi.status() == WL_CONNECTED) {
            Serial.printf("[WiFi] ✓ Connected! IP: %s\n", WiFi.localIP().toString().c_str());
            _was_connected = true;
            notifyStatus(true, "Connected");
            return true;
        }
        
        Serial.printf("[WiFi] ✗ Attempt %d failed\n", attempt);
        if (attempt < max_attempts) {
            Serial.println("[WiFi] Retrying in 3 seconds...");
            delay(3000);
        }
    }
    
    Serial.println("[WiFi] ✗ Connection failed after all attempts");
    notifyStatus(false, "Failed");
    return false;
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getIPAddress() {
    return WiFi.localIP().toString();
}

void WiFiManager::monitor() {
    if (millis() - _last_check < _check_interval) {
        return;
    }
    _last_check = millis();
    
    bool currently_connected = isConnected();
    if (_was_connected && !currently_connected) {
        Serial.println("[WiFi] Connection lost!");
        notifyStatus(false, "Disconnected");
    } else if (!_was_connected && currently_connected) {
        Serial.println("[WiFi] Reconnected!");
        notifyStatus(true, "Reconnected");
    }
    _was_connected = currently_connected;
}

void WiFiManager::setStatusCallback(wifi_status_callback_t callback) {
    _status_callback = callback;
}

void WiFiManager::disconnect() {
    WiFi.disconnect();
    _was_connected = false;
}

void WiFiManager::notifyStatus(bool connected, const char* message) {
    if (_status_callback) {
        _status_callback(connected, message);
    }
}
