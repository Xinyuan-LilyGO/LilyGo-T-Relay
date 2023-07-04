#include <Wire.h>
#include <WiFi.h>
#include <SPI.h>
#include <Arduino.h>
#include "SensorPCF8563.hpp"
#include <ShiftRegister74HC595_NonTemplate.h>
#include <AceButton.h>
#include <U8g2lib.h>
#include "sntp.h"

using namespace ace_button;

#define CH595

#define I2C_SDA                     16
#define I2C_SCL                     17
#define RTC_IRQ                     15

#define DATA_PIN                    7
#define CLOCK_PIN                   5
#define LATCH_PIN                   6


uint8_t configSize = 3;
ShiftRegister74HC595_NonTemplate     *control;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C *u8g2 = nullptr;
SensorPCF8563 rtc;
uint32_t lastMillis;

AceButton boot;
uint8_t state = 0;

#define wifi_ssid       "Your wifi ssid"
#define wifi_password   "Your wifi password"

const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";

// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t)
{
    Serial.println("Got time adjustment from NTP!");
    // printLocalTime();
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("No time available (yet)");
        return;
    }
    rtc.hwClockWrite();
    if (u8g2) {
        u8g2->clear();
    }
}


void ButtonHandleEvent(AceButton *n, uint8_t eventType, uint8_t buttonState)
{

    if (eventType != AceButton::kEventPressed) {
        return;
    }
    Serial.printf("[AceButton][%u]  N:%d E:%u S:%u state:%d\n", millis(), n->getId(), buttonState, buttonState, state);

    switch (state) {
    case 0:
        control->setAllHigh();
        break;
    case 1:
        control->setAllLow();
        break;
    case 2:
        // setting single pins
        for (int i = 0; i < configSize * 8; i++) {
            control->set(i, HIGH); // set single pin HIGH
            delay(250);
        }
        break;

    case 3:
        // setting single pins
        for (int i = (configSize * 8) - 1; i >= 0; i--) {
            control->set(i, LOW); // set single pin LOW
            delay(250);
        }
        break;
    default:
        break;
    }
    state++;
    state %= 4;

}
void setup()
{
    // Make sure that the driver chip output is set to LOW when powering on
    control = new ShiftRegister74HC595_NonTemplate(24, DATA_PIN, CLOCK_PIN, LATCH_PIN);
    assert(control);
    control->setAllLow();

    Serial.begin(115200);
    while (!Serial);

#ifdef RTC_IRQ
    pinMode(RTC_IRQ, INPUT_PULLUP);
#endif

    pinMode(0, INPUT_PULLUP);
    boot.init(0, HIGH, 0);
    ButtonConfig *buttonConfig = boot.getButtonConfig();
    buttonConfig->setEventHandler(ButtonHandleEvent);
    buttonConfig->setFeature(ButtonConfig::kFeatureClick);
    buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
    buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
    buttonConfig->setLongPressDelay(5000);

    Wire.begin(I2C_SDA, I2C_SCL);


    if (!rtc.begin(Wire, PCF8563_SLAVE_ADDRESS, I2C_SDA, I2C_SCL)) {
        Serial.println("Failed to find PCF8563 - check your wiring!");
    }


    Wire.beginTransmission(0x3C);
    if (Wire.endTransmission() == 0) {
        u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R2, U8X8_PIN_NONE);
        u8g2->begin();
        u8g2->clearBuffer();
        u8g2->setFlipMode(0);
        u8g2->setFontMode(1); // Transparent
        u8g2->setDrawColor(1);
        u8g2->setFontDirection(0);
        u8g2->firstPage();
        do {
            u8g2->setFont(u8g2_font_fub20_tf);
            u8g2->drawStr(15, 30, "LilyGo");
            u8g2->drawHLine(2, 35, 47);
            u8g2->drawHLine(3, 36, 47);
            u8g2->drawVLine(45, 32, 12);
            u8g2->drawVLine(46, 33, 12);
            u8g2->setFont(u8g2_font_fub14_tf);
            u8g2->drawStr(56, 53, "Relay");
        } while ( u8g2->nextPage() );
        u8g2->sendBuffer();
        u8g2->setFont(u8g2_font_fur11_tf);
    }


    WiFi.begin(wifi_ssid, wifi_password);

    // set notification call-back function
    sntp_set_time_sync_notification_cb( timeavailable );

    /**
     * A more convenient approach to handle TimeZones with daylightOffset
     * would be to specify a environmnet variable with TimeZone definition including daylight adjustmnet rules.
     * A list of rules for your zone could be obtained from https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
     */
    configTzTime("CST-8", ntpServer1, ntpServer2);

    if (WiFi.waitForConnectResult(0xFFFFFFF) != WL_CONNECTED) {
        Serial.println("Connect network failed !");
        if (u8g2) {
            u8g2->clear();
            u8g2->setFont(u8g2_font_timR10_tr);
            u8g2->setDrawColor(1);
            u8g2->drawBox(0, 0, u8g2->getWidth(), 14);
            u8g2->setDrawColor(0);
            u8g2->drawStr(0, 12, "Configure Network");
            u8g2->setDrawColor(1);
            u8g2->drawStr(0, 26, "Connect Failed!");
            u8g2->drawStr(0, 40, "Please check the ");
            u8g2->drawStr(0, 54, "connection params");
            u8g2->sendBuffer();
        }
    }

    if (u8g2) {
        u8g2->clear();
    }

}


void loop()
{
    boot.check();

    if (millis() - lastMillis > 1000) {

        lastMillis = millis();

        RTC_DateTime datetime = rtc.getDateTime();
        // Serial.printf(" Year :"); Serial.print(datetime.year);
        // Serial.printf(" Month:"); Serial.print(datetime.month);
        // Serial.printf(" Day :"); Serial.print(datetime.day);
        // Serial.printf(" Hour:"); Serial.print(datetime.hour);
        // Serial.printf(" Minute:"); Serial.print(datetime.minute);
        // Serial.printf(" Sec :"); Serial.println(datetime.second);

        if (u8g2) {
            // u8g2->clear();
            u8g2->setFont(u8g2_font_timR10_tr);
            u8g2->setDrawColor(1);
            u8g2->drawBox(0, 0, u8g2->getWidth(), 14);
            u8g2->setDrawColor(0);
            u8g2->drawStr(0, 12, "RTC");
            u8g2->setDrawColor(1);
            Wire.beginTransmission(0x51);
            if (Wire.endTransmission() != 0) {
                u8g2->drawStr(0, 26, "Failed to find RTC");
            } else {
                u8g2->drawStr(0, 26, (String(datetime.year) + "/" + String(datetime.month) + "/" + String(datetime.day)).c_str());


                u8g2->setDrawColor(0);
                u8g2->drawBox(0, 30, u8g2->getWidth(), 12);

                // u8g2->drawFrame(0, 40, 128, 64);
                u8g2->setDrawColor(1);
                u8g2->drawStr(0, 40, (String(datetime.hour) + "/" + String(datetime.minute) + "/" + String(datetime.second)).c_str());
            }
            u8g2->sendBuffer();
        }
    }
}



