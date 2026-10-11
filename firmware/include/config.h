#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// --- HARDWARE PIN DEFINITIONS ---
constexpr uint8_t RELAY_PIN = 26;   // GPIO connected to fan relay module
constexpr uint8_t I2C_SDA   = 21;   // ESP32 SDA
constexpr uint8_t I2C_SCL   = 22;   // ESP32 SCL

// --- NETWORK CONFIGURATION ---
extern const char* WIFI_SSID;
extern const char* WIFI_PASS;
extern const char* MQTT_SERVER;
constexpr uint16_t MQTT_PORT = 1883;

// --- MQTT TOPICS ---
#define TOPIC_TELEMETRY "home/bathroom/telemetry"
#define TOPIC_CONTROL   "home/bathroom/control/set"
#define TOPIC_CONFIG    "home/bathroom/config/set"

// --- ALGORITHM TUNING DEFAULTS ---
constexpr unsigned long SAMPLE_INTERVAL_MS = 5000;          // 5 seconds
constexpr float DEFAULT_EMA_ALPHA          = 0.02f;         // Exponential moving average rate
constexpr float DEFAULT_SPIKE_THRESHOLD    = 8.0f;          // % RH jump to trigger
constexpr float DEFAULT_DECAY_TARGET       = 0.60f;         // 60% recovery target
constexpr unsigned long DEFAULT_TIMEOUT_MS = 30 * 60 * 1000UL; // 30-min safety timeout

#endif // CONFIG_H