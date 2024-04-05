/**
 * @file      RTC_PCF8563_TimeSynchronization.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @date      2022-12-12
 * @note      PCF8563 is the onboard RTC
 */
#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>
#include <time.h>
#include <WiFi.h>
#include <sntp.h>
#include "SensorPCF8563.hpp"

#define I2C_SDA                     16
#define I2C_SCL                     17
#define RTC_IRQ                     18

const char *ssid       = "YOUR_SSID";
const char *password   = "YOUR_PASS";

const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

const char *time_zone = "CST-8";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)

SensorPCF8563 rtc;
uint32_t lastMillis;


// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t)
{
    Serial.println("Got time adjustment from NTP, Write the hardware clock");

    // Write synchronization time to hardware
    rtc.hwClockWrite();
}

void setup()
{
    Serial.begin(115200);
    while (!Serial);

    pinMode(RTC_IRQ, INPUT_PULLUP);

    if (!rtc.begin(Wire, PCF8563_SLAVE_ADDRESS, I2C_SDA, I2C_SCL)) {
        Serial.println("Failed to find PCF8563 - check your wiring!");
        while (1) {
            delay(1000);
        }
    }

    // set notification call-back function
    sntp_set_time_sync_notification_cb( timeavailable );

    /**
     * NTP server address could be aquired via DHCP,
     *
     * NOTE: This call should be made BEFORE esp32 aquires IP address via DHCP,
     * otherwise SNTP option 42 would be rejected by default.
     * NOTE: configTime() function call if made AFTER DHCP-client run
     * will OVERRIDE aquired NTP server address
     */
    sntp_servermode_dhcp(1);    // (optional)

    /**
     * This will set configured ntp servers and constant TimeZone/daylightOffset
     * should be OK if your time zone does not need to adjust daylightOffset twice a year,
     * in such a case time adjustment won't be handled automagicaly.
     */
    // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

    /**
     * A more convenient approach to handle TimeZones with daylightOffset
     * would be to specify a environmnet variable with TimeZone definition including daylight adjustmnet rules.
     * A list of rules for your zone could be obtained from https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
     */
    configTzTime(time_zone, ntpServer1, ntpServer2);

    //connect to WiFi
    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" CONNECTED");

}




void loop()
{
    if (millis() - lastMillis > 1000) {

        lastMillis = millis();

        // hardware clock
        struct tm hwTimeinfo;
        rtc.getDateTime(&hwTimeinfo);
        Serial.print("Hardware clock :");
        Serial.println(&hwTimeinfo, "%A, %B %d %Y %H:%M:%S");

        // system clock
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            Serial.println("No time available (yet)");
            return;
        }

        Serial.print("System   clock :");
        Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
        Serial.println();

    }
}



