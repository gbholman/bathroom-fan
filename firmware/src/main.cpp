#include <Arduino.h>
#include "config.h"
#include "sensor_manager.h"
#include "algorithm.h"
#include "mqtt_handler.h"

static unsigned long lastSampleTime = 0;

void setup() {
    Serial.begin(115200);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

    if (!initSensor()) {
        Serial.println("Failed to initialize SHT41 sensor!");
    }

    setupNetwork();
}

void loop() {
    handleMQTTLoop();

    unsigned long now = millis();
    if (now - lastSampleTime >= SAMPLE_INTERVAL_MS) {
        lastSampleTime = now;

        SensorData data = readSensor();
        if (data.readSuccess) {
            processTelemetry(data, now);
            publishTelemetry(data);
        }
    }
}