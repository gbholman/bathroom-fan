#include "algorithm.h"
#include "config.h"

SystemState state;

void turnFanOn(unsigned long now) {
    state.fanState = true;
    state.fanStartTime = now;
    digitalWrite(RELAY_PIN, HIGH);
}

void turnFanOff() {
    state.fanState = false;
    digitalWrite(RELAY_PIN, LOW);
}

void setControlMode(ControlMode newMode) {
    state.mode = newMode;
    if (newMode == MODE_MANUAL_ON) {
        turnFanOn(millis());
    } else if (newMode == MODE_MANUAL_OFF) {
        turnFanOff();
    }
}

void processTelemetry(const SensorData& sensorReading, unsigned long now) {
    if (!sensorReading.readSuccess) return;

    float currentRH = sensorReading.humidity;

    // 1. Manual Override Handling
    if (state.mode == MODE_MANUAL_ON) {
        digitalWrite(RELAY_PIN, HIGH);
        return;
    }
    if (state.mode == MODE_MANUAL_OFF) {
        digitalWrite(RELAY_PIN, LOW);
        return;
    }

    // 2. Frozen Baseline Rule
    if (!state.fanState) {
        // Update baseline using EMA only when fan is off
        state.baselineRH = (state.alpha * currentRH) + ((1.0f - state.alpha) * state.baselineRH);
        
        // Check for trigger conditions
        if ((currentRH - state.baselineRH) >= state.spikeThreshold) {
            state.peakRH = currentRH;
            turnFanOn(now);
        }
    } else {
        // Track peak humidity while running
        if (currentRH > state.peakRH) {
            state.peakRH = currentRH;
        }

        // Evaluate shutoff conditions
        float targetOff = state.peakRH - (state.decayTarget * (state.peakRH - state.baselineRH));
        bool decayReached = (currentRH <= targetOff) || (currentRH <= state.baselineRH + 2.0f);
        bool timedOut = (now - state.fanStartTime) >= state.timeoutMs;

        if (decayReached || timedOut) {
            turnFanOff();
        }
    }
}