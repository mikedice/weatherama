#include "TempSensor.h"


TempSensor::TempSensor(uint16_t dx)
{
    currentTempF = 0.0;
    currentTemp = 0.0;
    currentPress = 0.0;
    currentAltitude = 0.0;
    updateTempCounter = 0;
    delayCount = dx;
}

TempSensor::TempSensor() :
    TempSensor(DEFAULT_UPDATE_DELAY_COUNT)
{

}

void TempSensor::init()
{
    sensor.begin();
}

double TempSensor::getCurrentTempC()
{
    return this->currentTemp;
}

double TempSensor::getCurrentTempF()
{
    return this->currentTempF;
}

double TempSensor::getCurrentPressure()
{
    return this->currentPress;
}

double TempSensor::getCurrentAltitude()
{
    return this->currentAltitude;
}

void TempSensor::updateSensor()
{
    // The BMP180 sensor doesn't need to be read every time through the loop
    if (updateTempCounter > delayCount)
    {
        updateTemperature();
        updatePressure();
        updateAltitude();
        updateTempCounter = 0;
    }
    else
    {
        updateTempCounter++;
    }
}

void TempSensor::updateSensorImmediate()
{
    this->updateTemperature();
    this->updatePressure();
    this->updateAltitude();
}

void TempSensor::updateTemperature()
{
    char result = sensor.startTemperature();
    
    delay(result);

    result = sensor.getTemperature(currentTemp);
    currentTempF = (currentTemp * 1.8) + 32;
}

void TempSensor::updatePressure()
{
    char result = sensor.startPressure(1);

    delay(result);

    result = sensor.getPressure(currentPress, currentTemp);
}

void TempSensor::updateAltitude()
{
    currentAltitude = sensor.altitude(currentPress, SEA_LEVEL_PRESSURE);
}
