#include "sensor_manager.h"
#include "config.h"
#include <Wire.h>
#include <Adafruit_SHT4x.h>

static Adafruit_SHT4x sht4 = Adafruit_SHT4x();

bool initSensor() {
    Wire.begin(I2C_SDA, I2C_SCL);
    if (!sht4.begin(&Wire)) {
        return false;
    }
    sht4.setPrecision(SHT4X_HIGH_PRECISION);
    sht4.setHeater(SHT4X_NO_HEATER);
    return true;
}

SensorData readSensor() {
    SensorData data;
    sensors_event_t humidity, temp;
    if (sht4.getEvent(&humidity, &temp)) {
        data.humidity = humidity.relative_humidity;
        data.temperatureC = temp.temperature;
        data.readSuccess = true;
    } else {
        data.readSuccess = false;
    }
    return data;
}