#include "Display.h"

Display::Display() :
    tft(Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST))
{

            
}

void Display::init()
{
    tft.initR(INITR_BLACKTAB);
    tft.fillScreen(BACKGROUND_COLOR);
    tft.setTextSize(TEXT_SIZE);

    // Very important - include background color with text color. This causes the
    // pixels in the background to be filled in so you don't see flickering when a
    // new character is drawn over the top of an old one.
    tft.setTextColor(TEXT_COLOR, BACKGROUND_COLOR);
    tft.setRotation(SCREEN_ROTATION);
}

bool Display::printTimeOnLcd(DateTime& cur, int16_t x, int16_t y)
{
    x += 1 * TEXT_SIZE * 6;

    // when the transition from midnight happens the string shortens
    // so the field needs to be blanked
    if (cur.hours == 0 &&
        cur.minutes == 0 &&
        cur.seconds == 0)
    {
        tft.fillRect(0, y, tft.width(), TEXT_SIZE * 8, BACKGROUND_COLOR);
    }

    tft.setCursor(x, y);
    if (cur.hours <= 12)
    {
        tft.print(cur.hours);
    }
    else
    {
        tft.print(cur.hours - 12);
    }
    tft.print(":");
    if (cur.minutes < 10) tft.print("0");
    tft.print(cur.minutes);
    tft.print(":");
    if (cur.seconds < 10) tft.print("0");
    tft.print(cur.seconds);

    if (cur.hours <= 12) tft.print(" AM");
    else tft.print(" PM");
}

bool Display::printDateOnLcd(DateTime& cur, int16_t x, int16_t y)
{
    bool isAM = cur.hours <= 12;

    y += LINE_HEIGHT;
    x += 1 * TEXT_SIZE * 6;
    tft.setCursor(x, y);

    if (cur.hours == 0 &&
        cur.minutes == 0 &&
        cur.seconds == 0)
    {
        tft.fillRect(0, y, tft.width(), TEXT_SIZE * 8, BACKGROUND_COLOR);
    }
    switch (cur.wday)
    {
    case 1:
        tft.print("Sunday");
        break;
    case 2:
        tft.print("Monday");
        break;
    case 3:
        tft.print("Tuesday");
        break;
    case 4:
        tft.print("Wedensday");
        break;
    case 5:
        tft.print("Thursday");
        break;
    case 6:
        tft.print("Friday");
        break;
    case 7:
        tft.print("Saturday");
        break;
    }

    y += LINE_HEIGHT;
    tft.setCursor(x, y);

    if (cur.hours == 0 &&
        cur.minutes == 0 &&
        cur.seconds == 0)
    {
        tft.fillRect(0, y, tft.width(), TEXT_SIZE * 8, BACKGROUND_COLOR);
    }

    switch (cur.month)
    {
    case 1:
        tft.print("Jan");
        break;
    case 2:
        tft.print("Feb");
        break;
    case 3:
        tft.print("Mar");
        break;
    case 4:
        tft.print("Apr");
        break;
    case 5:
        tft.print("May");
        break;
    case 6:
        tft.print("Jun");
        break;
    case 7:
        tft.print("Jul");
        break;
    case 8:
        tft.print("Aug");
        break;
    case 9:
        tft.print("Sep");
        break;
    case 10:
        tft.print("Oct");
        break;
    case 11:
        tft.print("Nov");
        break;
    case 12:
        tft.print("Dec");
        break;
    }

    tft.print(" ");
    tft.print(cur.day);
    tft.print(" ");
    tft.print(cur.year);
}

void Display::printTempOnLcd(double temp, int16_t x, int16_t y)
{
    y += LINE_HEIGHT * 3 + 10;
    x += 1 * TEXT_SIZE * 6;

    tft.setCursor(x, y);
    tft.print("T: ");
    tft.print(temp, 1);
    tft.print("F");
}

void Display::printPressureOnLcd(double pressure, int16_t x, int16_t y)
{
    y += LINE_HEIGHT * 4 + 10;
    x += 1 * TEXT_SIZE * 6;

    tft.setCursor(x, y);
    tft.print("B: ");
    tft.print(pressure, 1);
    tft.print("mb");
}

void Display::printAltitudeOnLcd(double altitude, int16_t x, int16_t y)
{
    y += LINE_HEIGHT * 5 + 10;
    x += 1 * TEXT_SIZE * 6;

    tft.setCursor(x, y);
    tft.print("A: ");
    tft.print(altitude, 1);
}