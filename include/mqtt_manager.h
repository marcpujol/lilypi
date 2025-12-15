#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

class MQTTManager {
public:
    MQTTManager();
    
    // Initialize MQTT with server details
    void begin(const char* server, uint16_t port, const char* clientId);
    
    // Connect to MQTT broker
    bool connect();
    
    // Check if connected
    bool isConnected();
    
    // Subscribe to a topic
    bool subscribe(const char* topic);
    
    // Subscribe to multiple topics
    bool subscribeMultiple(const char* topics[], int count);
    
    // Publish a message
    bool publish(const char* topic, const char* payload, bool retained = false);
    
    // Process incoming messages (call in loop)
    void loop();
    
    // Set callback for incoming messages
    void setCallback(void (*callback)(char*, uint8_t*, unsigned int));
    
    // Disconnect from broker
    void disconnect();

private:
    WiFiClient wifiClient;
    PubSubClient mqttClient;
    const char* server;
    uint16_t port;
    const char* clientId;
    
    // Reconnect helper
    bool reconnect();
};

#endif // MQTT_MANAGER_H
