#pragma once
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <Wire\Wire.h>
#include "Clock.h"


// tft display constants
#define LINE_HEIGHT 20
#define BACKGROUND_COLOR ST7735_BLACK
#define TEXT_COLOR ST7735_GREEN
#define TEXT_SIZE 2
#define SCREEN_ROTATION 1
#define TFT_CS     10
#define TFT_RST    8
#define TFT_DC     9


class Display
{
public:
    Display();
    void init();
    bool printTimeOnLcd(DateTime& cur, int16_t x, int16_t y);
    bool printDateOnLcd(DateTime& cur, int16_t x, int16_t y);
    void printTempOnLcd(double temp, int16_t x, int16_t y);
    void printPressureOnLcd(double pressure, int16_t x, int16_t y);
    void printAltitudeOnLcd(double altitude, int16_t x, int16_t y);
private:
    Adafruit_ST7735 tft; 
};