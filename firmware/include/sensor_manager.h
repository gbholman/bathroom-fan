#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

struct SensorData {
    float humidity = 0.0f;
    float temperatureC = 0.0f;
    bool readSuccess = false;
};

bool initSensor();
SensorData readSensor();

#endif // SENSOR_MANAGER_H