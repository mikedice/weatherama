#include "Display.h"

DateStringEntry DaysOfWeek[] =
{
    { "Sunday",  sizeof("Sunday") },
    { "Monday", sizeof("Monday") },
    { "Tuesday", sizeof("Tuesday") },
    { "Wedensday", sizeof("Wedensday") },
    { "Thursday", sizeof("Thursday") },
    { "Friday", sizeof("Friday") },
    { "Saturday", sizeof("Saturday") },
};

DateStringEntry MonthsOfYear[] =
{
    { "Jan",  sizeof("Jan") },
    { "Feb",  sizeof("Feb") },
    { "Mar",  sizeof("Mar") },
    { "Apr",  sizeof("Apr") },
    { "May",  sizeof("May") },
    { "Jun",  sizeof("Jun") },
    { "Jul",  sizeof("Jul") },
    { "Aug",  sizeof("Aug") },
    { "Sep",  sizeof("Sep") },
    { "Oct",  sizeof("Oct") },
    { "Nov",  sizeof("Nov") },
    { "Dec",  sizeof("Dec") },
};

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
    // 1 character margin
    x += 1 * TEXT_SIZE * 6;

    // we can have 12 characters
    unsigned char buffer[LINE_BUFFER_SIZE] = { 0 };
    int index = 0;
    uint8_t hour = 0;

    // AM/PM calculation
    if (cur.hours > 12)
    {
        hour = cur.hours - 12;
    }
    else
    {
        hour = cur.hours;
    }

    if (hour < 10)
    {
        buffer[index++] = hour + '0';
    }
    else
    {
        buffer[index++] = (hour / 10) + '0';
        buffer[index++] = (hour % 10) + '0';
    }

    buffer[index++] = ':';

    if (cur.minutes < 10)
    {
        buffer[index++] = '0';
        buffer[index++] = cur.minutes + '0';
    }
    else
    {
        buffer[index++] = (cur.minutes / 10) + '0';
        buffer[index++] = (cur.minutes % 10) + '0';
    }

    buffer[index++] = ':';

    if (cur.seconds < 10)
    {
        buffer[index++] = '0';
        buffer[index++] = cur.seconds + '0';
    }
    else
    {
        buffer[index++] = (cur.seconds / 10) + '0';
        buffer[index++] = (cur.seconds % 10) + '0';
    }

    buffer[index++] = ' ';

    if (cur.hours < 12)
    {
        buffer[index++] = 'A';
    }
    else
    {
        buffer[index++] = 'P';
    }

    buffer[index++] = 'M';

    fillInAndPrint(buffer, index, x, y);
}

bool Display::printDateOnLcd(DateTime& cur, int16_t x, int16_t y)
{
    bool isAM = cur.hours <= 12;

    y += LINE_HEIGHT;
    x += 1 * TEXT_SIZE * 6;
    tft.setCursor(x, y);

    unsigned char buffer[LINE_BUFFER_SIZE] = { 0 };
    int index = 0;

    memcpy(buffer, DaysOfWeek[cur.wday - 1].pString, DaysOfWeek[cur.wday - 1].length);
    fillInAndPrint(buffer, DaysOfWeek[cur.wday - 1].length, x, y);

    y += LINE_HEIGHT;
    tft.setCursor(x, y);

    memset(buffer, 0, sizeof(buffer) / sizeof(buffer[0]));
    memcpy(buffer, MonthsOfYear[cur.month - 1].pString, MonthsOfYear[cur.month - 1].length);
    index = 3;
    buffer[index++] = ' ';
    if (cur.day < 10)
    {
        buffer[index++] = '0' + cur.day;
    }
    else
    {
        buffer[index++] = '0' + (cur.day / 10);
        buffer[index++] = '0' + (cur.day % 10);
    }

    buffer[index++] = ' ';

    int tempYear = cur.year;
    if (tempYear < 2000)
    {
        return false;
    }
    buffer[index++] = '0' + (tempYear / 1000);
    tempYear -= (tempYear / 1000) * 1000;

    buffer[index++] = '0' + (tempYear / 100);
    tempYear -= (tempYear / 100) * 100;

    buffer[index++] = '0' + (tempYear / 10);
    tempYear -= (tempYear / 10) * 10;

    buffer[index++] = '0' + tempYear % 10;

    fillInAndPrint(buffer, index, x, y);
}

void Display::printTempOnLcd(double temp, int16_t x, int16_t y)
{
    y += LINE_HEIGHT * 3 + 10;
    x += 1 * TEXT_SIZE * 6;

    tft.setCursor(x, y);
    unsigned char buffer[LINE_BUFFER_SIZE] = { 0 };
    int index = 0;
    buffer[index++] = 'T';
    buffer[index++] = ':';
    buffer[index++] = ' ';

    index = bufferFloat(temp, buffer, index, 1);
    buffer[index++] = 'F';
    fillInAndPrint(buffer, index, x, y);
}

void Display::printPressureOnLcd(double pressure, int16_t x, int16_t y)
{
    y += LINE_HEIGHT * 4 + 10;
    x += 1 * TEXT_SIZE * 6;

    tft.setCursor(x, y);
    unsigned char buffer[LINE_BUFFER_SIZE] = { 0 };
    int index = 0;
    buffer[index++] = 'B';
    buffer[index++] = ':';
    buffer[index++] = ' ';

    index = bufferFloat(pressure, buffer, index, 1);
    buffer[index++] = 'm';
    buffer[index++] = 'b';
    fillInAndPrint(buffer, index, x, y);

}

void Display::printAltitudeOnLcd(double altitude, int16_t x, int16_t y)
{
    y += LINE_HEIGHT * 5 + 10;
    x += 1 * TEXT_SIZE * 6;

    tft.setCursor(x, y);
    tft.print("A: ");
    tft.print(altitude, 1);
}

void Display::fillInAndPrint(unsigned char* buffer, int contentSize, int16_t x, int16_t y)
{
    // fill in with spaces
    for (int i = contentSize; i < LINE_BUFFER_SIZE; i++)
    {
        buffer[i] = ' ';
    }

    // print characters one at a time
    for (int i = 0; i < LINE_BUFFER_SIZE; i++)
    {
        tft.drawChar(x, y, buffer[i], TEXT_COLOR, BACKGROUND_COLOR, TEXT_SIZE);
        x += TEXT_SIZE * 6;
    }
}

int Display::bufferFloat(double number, unsigned char* buffer, int index, uint8_t digits)
{

    if (isnan(number)) return -1; // nan
    if (isinf(number)) return -1; // inf
    if (number > 4294967040.0) return -1; // ovf
    if (number < -4294967040.0) return -1; // ovf

                                                     // Handle negative numbers
    if (number < 0.0)
    {
        buffer[index++] = '-';
        number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for (uint8_t i = 0; i<digits; ++i)
        rounding /= 10.0;

    number += rounding;

    // Extract the integer part of the number and print it
    unsigned long int_part = (unsigned long)number;
    double remainder = number - (double)int_part;
    char numBuffer[16];
    ltoa((long)int_part, numBuffer, 10);
    size_t len = strlen(numBuffer);
    for (size_t i = 0; i < len; i++)
    {
        buffer[index++] = numBuffer[i];
    }

    // Print the decimal point, but only if there are digits beyond
    if (digits > 0) {
        buffer[index++] = '.';
    }

    // Extract digits from the remainder one at a time
    while (digits-- > 0)
    {
        remainder *= 10.0;
        int toPrint = int(remainder);
        buffer[index++] = '0' + toPrint;
        remainder -= toPrint;
    }
    return index;
}