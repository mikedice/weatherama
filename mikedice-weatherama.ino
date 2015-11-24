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
#include <IPAddress.h>
#define LOOP_MS 250

// Little hardware thingies used by the project
Clock clock;
TempSensor tempSensor;
Display display;
DateTime currentTime;

// CC3000 interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3 // MUST be an interrupt pin!
#define ADAFRUIT_CC3000_VBAT  5 // These can be
#define ADAFRUIT_CC3000_CS   10 // any two pins
// Hardware SPI required for remaining pins.
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(
    ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
    SPI_CLOCK_DIVIDER);
const char* wifiSSID = "proton";
const char* wifiPass = "kona1234";
#define WEBUPDATE_DELAY_LOOPCOUNT  4 * 5
uint8_t webpage_delay = 0;

IPAddress testServer(68, 1, 168, 192);

#define WEBSITE      "www.weatherama.com"
#define WEBPAGE      "/ArduinoHello"
#define IDLE_TIMEOUT_MS  2000  
bool networkReady = false;

void initializeWiFi()
{
    cc3000.begin();
    uint8_t majorVersion = 0;
    uint8_t minorVersion = 0;
    bool dhcpComplete = false;
    uint8_t rssi = 0;
    uint8_t secMode = 0;
    char networkName[64] = { 0 };
    bool networkFound = false;
    uint32_t numNetworks = 0;
    bool connected = false;

    cc3000.getFirmwareVersion(&majorVersion, &minorVersion);
    Serial.print("firmware: ");
    Serial.print(majorVersion);
    Serial.print(".");
    Serial.println(minorVersion);

    Serial.println("Scanning networks...");
    cc3000.startSSIDscan(&numNetworks);
    Serial.print("found ");
    Serial.print(numNetworks);
    Serial.println(" networks");

    for (uint32_t i = 0; i < numNetworks; i++)
    {
        rssi = 0;
        secMode = 0;
        memset(networkName, 0, sizeof(networkName)*sizeof(networkName[0]));
        cc3000.getNextSSID(&rssi, &secMode, networkName);
        Serial.println(networkName);
        if (strcmp(networkName, wifiSSID) == 0)
        {
            networkFound = true;
        }
    }

    if (networkFound)
    {
        Serial.println("Network found, connecting to AP");
        connected = cc3000.connectToAP(wifiSSID, wifiPass, secMode, 5);
        if (connected)
        {
            Serial.print("Connected to AP ");
            Serial.println(wifiSSID);

            Serial.println("Checking for DHCP");

            unsigned long start = millis();

            do
            {
                if (cc3000.checkDHCP())
                {
                    Serial.print("DHCP completed in ");
                    Serial.print(millis() - start);
                    Serial.println(" milliseconds");
                       
                    dhcpComplete = true;
                    break;
                }
                if (millis() - start > 120000)
                {
                    Serial.println("DHCP timed out after 120 seconds");
                    break;
                }
                delay(100);
            } while (!dhcpComplete);

            if (dhcpComplete)
            {
                uint32_t ipAddress = 0;
                uint32_t netMask = 0;
                uint32_t gateway = 0;
                uint32_t dhcpSrv = 0;
                uint32_t dnsSrv = 0;
                if (cc3000.getIPAddress(&ipAddress, &netMask, &gateway, &dhcpSrv, &dnsSrv))
                {
                    Serial.print("IPAddress: ");
                    cc3000.printIPdotsRev(ipAddress);
                    Serial.println();

                    Serial.print("DNS Server: ");
                    cc3000.printIPdotsRev(dnsSrv);
                    Serial.println();

                    Serial.print("Net Mask: ");
                    cc3000.printIPdotsRev(netMask);
                    Serial.println();

                    Serial.print("Gateway: ");
                    cc3000.printIPdotsRev(gateway);
                    Serial.println();

                    Serial.print("DHCP Server: ");
                    cc3000.printIPdotsRev(dhcpSrv);
                    Serial.println();
                    networkReady = true;
                }
                else
                {
                    Serial.println("Did not get IP Address");
                }
            }
        }
        else
        {
            Serial.println("Could not connect to AP");
        }
    }
}

void retrieveWebPage()
{
    if (networkReady)
    {
        //uint32_t ip = testServer;
        
        uint32_t ip = 0;
        // Try looking up the website's IP address
        Serial.print(WEBSITE); Serial.print(F(" -> "));
        while (ip == 0) {
        if (!cc3000.getHostByName(WEBSITE, &ip)) {
        Serial.println(F("Couldn't resolve!"));
        }
        delay(500);
        }


        Serial.print("Connecting to server at ip address: ");
        cc3000.printIPdotsRev(ip);
        Serial.println();

        // Optional: Do a ping test on the website
        /*
        Serial.print(F("\n\rPinging ")); cc3000.printIPdotsRev(ip); Serial.print("...");
        replies = cc3000.ping(ip, 5);
        Serial.print(replies); Serial.println(F(" replies"));
        */

        /* Try connecting to the website.
        Note: HTTP/1.1 protocol is used to keep the server from closing the connection before all data is read.
        */
        Adafruit_CC3000_Client www = cc3000.connectTCP(ip, 8001);
        if (www.connected()) {
            www.fastrprint(F("GET "));
            www.fastrprint(WEBPAGE);
            www.fastrprint(F(" HTTP/1.1\r\n"));
            www.fastrprint(F("Host: ")); www.fastrprint(WEBSITE); www.fastrprint(F("\r\n"));
            www.fastrprint(F("\r\n"));
            www.println();
        }
        else {
            Serial.println(F("Connection failed"));
            return;
        }

        Serial.println(F("-------------------------------------"));

        /* Read data until either the connection is closed, or the idle timeout is reached. */
        unsigned long lastRead = millis();
        while (www.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
            while (www.available()) {
                char c = www.read();
                Serial.print(c);
                lastRead = millis();
            }
        }
        www.close();
        Serial.println(F("-------------------------------------"));
        Serial.print("last web page access at ");
        Serial.print(currentTime.hours); Serial.print(":"); Serial.print(currentTime.minutes); Serial.print(":"); Serial.println(currentTime.seconds);
        delay(5000);

        /*
        if (connected)
        {
        cc3000.disconnect();
        Serial.print("Disconnected from AP ");
        Serial.println(wifiSSID);
        }
        */
    }
    else
    {
        Serial.println("Could not retrieve web page because network is not ready");
    }

}



void setup()
{
    currentTime.hours = currentTime.minutes = currentTime.seconds = 0;

    Serial.begin(115200);
    initializeWiFi();
    tempSensor.init();
    clock.init();
    display.init();
    tempSensor.updateSensorImmediate();

    Serial.println("weatherama setup completed.");

    // TODO: come up with a way to set the date/time other than using code

    DateTime current;
    current.seconds = 00;
    current.minutes = 39;
    current.hours = 17;
    current.wday = 7;
    current.month = 11;
    current.day = 22;
    current.year = 2015;
    clock.setDateTime(current);




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

    if (webpage_delay > WEBUPDATE_DELAY_LOOPCOUNT)
    {
        retrieveWebPage();
        webpage_delay = 0;
    }
    else
    {
        webpage_delay++;
    }

    delay(LOOP_MS);
}



