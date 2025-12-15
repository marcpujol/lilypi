#include "mqtt_manager.h"

MQTTManager::MQTTManager() : mqttClient(wifiClient) {
    server = nullptr;
    port = 1883;
    clientId = nullptr;
}

void MQTTManager::begin(const char* server, uint16_t port, const char* clientId) {
    this->server = server;
    this->port = port;
    this->clientId = clientId;
    
    mqttClient.setServer(server, port);
    
    Serial.println("MQTT Manager initialized");
    Serial.print("  Server: ");
    Serial.println(server);
    Serial.print("  Port: ");
    Serial.println(port);
    Serial.print("  Client ID: ");
    Serial.println(clientId);
}

bool MQTTManager::connect() {
    if (server == nullptr || clientId == nullptr) {
        Serial.println("❌ MQTT: Not initialized");
        return false;
    }
    
    if (mqttClient.connected()) {
        return true;
    }
    
    Serial.print("Connecting to MQTT broker... ");
    
    if (mqttClient.connect(clientId)) {
        Serial.println("✓ Connected!");
        return true;
    } else {
        Serial.print("❌ Failed, rc=");
        Serial.println(mqttClient.state());
        return false;
    }
}

bool MQTTManager::reconnect() {
    if (!mqttClient.connected()) {
        Serial.print("Attempting MQTT reconnection... ");
        
        if (mqttClient.connect(clientId)) {
            Serial.println("✓ Reconnected!");
            return true;
        } else {
            Serial.print("❌ Failed, rc=");
            Serial.println(mqttClient.state());
            return false;
        }
    }
    return true;
}

bool MQTTManager::isConnected() {
    return mqttClient.connected();
}

bool MQTTManager::subscribe(const char* topic) {
    if (!mqttClient.connected()) {
        Serial.println("❌ MQTT: Not connected, cannot subscribe");
        return false;
    }
    
    Serial.print("Subscribing to topic: ");
    Serial.print(topic);
    
    if (mqttClient.subscribe(topic)) {
        Serial.println(" ✓");
        return true;
    } else {
        Serial.println(" ❌");
        return false;
    }
}

bool MQTTManager::subscribeMultiple(const char* topics[], int count) {
    if (!mqttClient.connected()) {
        Serial.println("❌ MQTT: Not connected, cannot subscribe");
        return false;
    }
    
    Serial.println("Subscribing to multiple topics:");
    bool allSuccess = true;
    
    for (int i = 0; i < count; i++) {
        Serial.print("  - ");
        Serial.print(topics[i]);
        
        if (mqttClient.subscribe(topics[i])) {
            Serial.println(" ✓");
        } else {
            Serial.println(" ❌");
            allSuccess = false;
        }
    }
    
    return allSuccess;
}

bool MQTTManager::publish(const char* topic, const char* payload, bool retained) {
    if (!mqttClient.connected()) {
        Serial.println("❌ MQTT: Not connected, cannot publish");
        return false;
    }
    
    return mqttClient.publish(topic, payload, retained);
}

void MQTTManager::loop() {
    if (!mqttClient.connected()) {
        reconnect();
    }
    mqttClient.loop();
}

void MQTTManager::setCallback(void (*callback)(char*, uint8_t*, unsigned int)) {
    mqttClient.setCallback(callback);
}

void MQTTManager::disconnect() {
    if (mqttClient.connected()) {
        mqttClient.disconnect();
        Serial.println("MQTT disconnected");
    }
}
