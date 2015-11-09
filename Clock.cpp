#include <stdint.h>
#include <HardwareSerial.h>
#include <Wire\Wire.h>
#include <LiquidCrystal.h>
#include "Clock.h"

Clock::Clock()
{

}

void Clock::init()
{
    setupClock();
}

void Clock::readDateTime(DateTime& result)
{
    setupClock();
    wire.requestFrom(CLOCK_ADDR, 0x07);

    result.seconds = fromBcd(wire.read() & SECONDS_REGISTER_MASK);
    result.minutes = fromBcd(wire.read());
    result.hours = fromBcd(wire.read());
    result.wday = wire.read();
    result.day = fromBcd(wire.read());
    result.month = fromBcd(wire.read());
    result.year = fromBcd(wire.read()) + 2000;
}

void Clock::setDateTime(DateTime& dateTime)
{
    wire.beginTransmission(CLOCK_ADDR);
    wire.write(0x00); // tell clock to start writing at register 0;
    wire.write(toBcd(dateTime.seconds));
    wire.write(toBcd(dateTime.minutes));
    wire.write(toBcd(dateTime.hours));
    wire.write(dateTime.wday);
    wire.write(toBcd(dateTime.day));
    wire.write(toBcd(dateTime.month));
    wire.write(toBcd(dateTime.year - 2000));
    wire.endTransmission();
}

uint8_t Clock::toBcd(uint8_t value) {
    if (value >= 0 && value <= 99)
    {
        return ((value / 10) * 16) + (value % 10);
    }
    return 0;
}

uint8_t Clock::fromBcd(uint8_t bcd)
{
    return (bcd / 16 * 10) + (bcd % 16);
}

bool Clock::setupClock()
{
    wire.begin();
    wire.beginTransmission(CLOCK_ADDR);
    wire.write(0x00);
    uint8_t result = wire.endTransmission(CLOCK_ADDR);
    return result == 0x00;
}

