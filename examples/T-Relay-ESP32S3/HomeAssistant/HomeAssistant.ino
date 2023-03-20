/**
 * @file      main.cpp
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-02-21
 *
 */


#include <Arduino.h>
#include <WiFi.h>
#include "devices.h"

ShiftRegister74HC595_NonTemplate *control = NULL;
Ticker ledTick;
uint32_t running = 0;

extern void setupI2CDevices();
extern void setupWM();
extern bool setupHA(const char *ipaddress,
                    const uint16_t port,
                    const char *username,
                    const char *password);
extern void loopHA();

void setup()
{
    // Make sure that the driver chip output is set to LOW when powering on
    control = new ShiftRegister74HC595_NonTemplate(8, DATA_PIN, CLOCK_PIN, LATCH_PIN);
    assert(control);
    control->setAllLow();


    Serial.begin(115200);
    while ((!Serial)) {
    }

    setupI2CDevices();

    setupWM();

    if (control) {
        delete control;
    }

    control = new ShiftRegister74HC595_NonTemplate(setting.shilfNum * 8, DATA_PIN, CLOCK_PIN, LATCH_PIN);

    if (strlen(setting.username) != 0 || strlen(setting.password) != 0) {
        setupHA(setting.ipAddress, setting.serverPort,
                setting.username, setting.password);
    } else {
        setupHA(setting.ipAddress, setting.serverPort,
                NULL, NULL);
    }
}

void loop()
{
    if (millis() - running > 1000) {
        running = millis();

        char buf[64];
        struct tm timeinfo;
        // Get the time C library structure
        rtc.getDateTime(&timeinfo);
        // Format the output using the strftime function
        // For more formats, please refer to :
        // https://man7.org/linux/man-pages/man3/strftime.3.html
        size_t written = strftime(buf, 64, "%A, %B %d %Y %H:%M:%S", &timeinfo);
        if (written != 0) {
            Serial.println(buf);
        }
    }
    loopHA();
}
