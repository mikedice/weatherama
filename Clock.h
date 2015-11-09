#pragma once
#define CLOCK_ADDR 0x68 // I2C address of clock from datasheet at http://datasheets.maximintegrated.com/en/ds/DS1307.pdf
#define SECONDS_REGISTER_MASK 0x7F

struct DateTime
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t wday;
    uint8_t day;
    uint8_t month;
    uint16_t year;
};

class Clock
{
    
public:
    Clock();
    void readDateTime(DateTime& result);
    void setDateTime(DateTime& dateTime);
    void init();
private:
    uint8_t toBcd(uint8_t value);
    uint8_t fromBcd(uint8_t bcd);
    bool setupClock();
    TwoWire wire;
};


