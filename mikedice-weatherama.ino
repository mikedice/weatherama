#include <SFE_BMP180.h>
#include <Wire\Wire.h>
#include <LiquidCrystal.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include "Clock.h"
#include "TempSensor.h"
#include "Display.h"
#define LOOP_MS 250

// Little hardware thingies used by the project
Clock clock;
TempSensor tempSensor;
Display display;
DateTime currentTime;

void setup()
{
    currentTime.hours = currentTime.minutes = currentTime.seconds = 0;

    Serial.begin(115200);
    tempSensor.init();
    clock.init();
    display.init();
    tempSensor.updateSensorImmediate();
    
    Serial.println("setup completed.");

    // TODO: come up with a way to set the date/time other than using code
    /*
    DateTime current;
    current.seconds = 00;
    current.minutes = 01;
    current.hours = 11;
    current.wday = 7;
    current.month = 11;
    current.day = 7;
    current.year = 2015;
    clock.setDateTime(current);
    */
}

void loop()
{
    // update readings from sensors & clock
    clock.readDateTime(currentTime);
    tempSensor.updateSensor();

    // Update the TFT screen with new values.
    int16_t tftStartY = 5;
    int16_t tftStartX = 5;
    display.printTimeOnLcd(currentTime, tftStartX, tftStartY);
    display.printDateOnLcd(currentTime, tftStartX, tftStartY);
    display.printTempOnLcd(tempSensor.getCurrentTempF(), tftStartX, tftStartY);
    display.printPressureOnLcd(tempSensor.getCurrentPressure(), tftStartX, tftStartY);

    delay(LOOP_MS);
}
