#include "mqtt_handler.h"
#include "config.h"
#include "algorithm.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";
const char* MQTT_SERVER = "192.168.1.100";

static WiFiClient espClient;
static PubSubClient mqttClient(espClient);

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, payload, length);
    if (err) return;

    if (String(topic) == TOPIC_CONTROL) {
        const char* cmd = doc["command"] | "";
        if (strcmp(cmd, "AUTO") == 0) setControlMode(MODE_AUTO);
        else if (strcmp(cmd, "MANUAL_ON") == 0) setControlMode(MODE_MANUAL_ON);
        else if (strcmp(cmd, "MANUAL_OFF") == 0) setControlMode(MODE_MANUAL_OFF);
    } else if (String(topic) == TOPIC_CONFIG) {
        if (doc.containsKey("spike")) state.spikeThreshold = doc["spike"];
        if (doc.containsKey("decay")) state.decayTarget = doc["decay"];
        if (doc.containsKey("timeout")) state.timeoutMs = (unsigned long)doc["timeout"] * 60 * 1000;
    }
}

void setupNetwork() {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setCallback(mqttCallback);
}

void handleMQTTLoop() {
    if (!mqttClient.connected()) {
        if (mqttClient.connect("ESP32_Bathroom_Controller")) {
            mqttClient.subscribe(TOPIC_CONTROL);
            mqttClient.subscribe(TOPIC_CONFIG);
        }
    } else {
        mqttClient.loop();
    }
}

void publishTelemetry(const SensorData& sensorReading) {
    if (!mqttClient.connected()) return;

    JsonDocument doc;
    doc["humidity"] = sensorReading.humidity;
    doc["temperature"] = sensorReading.temperatureC;
    doc["baseline"] = state.baselineRH;
    doc["fan_state"] = state.fanState;
    doc["mode"] = state.mode == MODE_AUTO ? "AUTO" : (state.mode == MODE_MANUAL_ON ? "MANUAL_ON" : "MANUAL_OFF");

    char buffer[256];
    serializeJson(doc, buffer);
    mqttClient.publish(TOPIC_TELEMETRY, buffer);
}