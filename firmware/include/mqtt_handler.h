#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include "sensor_manager.h"

void setupNetwork();
void handleMQTTLoop();
void publishTelemetry(const SensorData& sensorReading);

#endif // MQTT_HANDLER_H