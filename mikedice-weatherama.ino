#include <ccspi.h>
#include <Adafruit_CC3000_Server.h>
#include <Adafruit_CC3000.h>
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

// CC3000 interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3 // MUST be an interrupt pin!
#define ADAFRUIT_CC3000_VBAT  5 // These can be
#define ADAFRUIT_CC3000_CS   10 // any two pins
// Hardware SPI required for remaining pins.
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(
    ADAFRUIT_CC3000_CS, 
    ADAFRUIT_CC3000_IRQ, 
    ADAFRUIT_CC3000_VBAT,
    SPI_CLOCK_DIVIDER);
const char* wifiSSID = "proton";
const char* wifiPass = "kona1234";
int wifiUpdateCount = 0;
#define WIFI_COUNT 20

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

    Serial.println("weatherama setup completed.");

    // TODO: come up with a way to set the date/time other than using code
    /*
    DateTime current;
    current.seconds = 00;
    current.minutes = 17;
    current.hours = 21;
    current.wday = 6;
    current.month = 11;
    current.day = 21;
    current.year = 2015;
    clock.setDateTime(current);
    */



}

//void wifiSendData(double temp, double press, DateTime currentDt)
//{
//    Serial.println("attempting to connect to server");
//    Adafruit_CC3000_Client client;
//    IPAddress addr(192,168,1,68);
//    int connectResult = cc3000.connectTCP(addr, 52476);
//    if (connectResult > 0)
//    {
//        Serial.print("succeeded");
//        Serial.println(connectResult);
//        client.close();
//    }
//    else
//    {
//        Serial.println("Failed ");
//        Serial.println(connectResult);
//    }
//}

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

    //if (wifiUpdateCount > WIFI_COUNT)
    //{
    //    wifiSendData(tempSensor.getCurrentTempF(), tempSensor.getCurrentPressure(), currentTime);
    //    wifiUpdateCount = 0;
    //}
    //else
    //{
    //    wifiUpdateCount++;
    //}

    delay(LOOP_MS);
}



