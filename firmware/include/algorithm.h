#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "sensor_manager.h"
#include <Arduino.h>

enum ControlMode { MODE_AUTO, MODE_MANUAL_ON, MODE_MANUAL_OFF };

struct SystemState {
    float baselineRH = 50.0f;
    float peakRH = 50.0f;
    float rateOfChange = 0.0f;
    bool fanState = false;
    ControlMode mode = MODE_AUTO;
    unsigned long fanStartTime = 0;
    
    // Dynamic config overrides
    float spikeThreshold = 8.0f;
    float decayTarget = 0.60f;
    unsigned long timeoutMs = 30 * 60 * 1000UL;
    float alpha = 0.02f;
};

extern SystemState state;

void processTelemetry(const SensorData& sensorReading, unsigned long now);
void setControlMode(ControlMode newMode);
void turnFanOn(unsigned long now);
void turnFanOff();

#endif // ALGORITHM_H