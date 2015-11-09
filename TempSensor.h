#pragma once
#include <SFE_BMP180.h>
#define DEFAULT_UPDATE_DELAY_COUNT 20
#define SEA_LEVEL_PRESSURE 1013.25 // From Wikipedia

// Temperature sensor based on a BMP180 made by Bosch
class TempSensor
{
public:
    TempSensor();
    TempSensor(uint16_t delayCount);
    void init();
    void updateSensor();
    void updateSensorImmediate();
    double getCurrentTempC();
    double getCurrentTempF();
    double getCurrentPressure();
    double getCurrentAltitude();

private:
    void updateTemperature();
    void updatePressure();
    void updateAltitude();

    SFE_BMP180 sensor;
    double currentTempF;
    double currentTemp;
    double currentPress;
    double currentAltitude;
    int updateTempCounter;
    int delayCount;
};