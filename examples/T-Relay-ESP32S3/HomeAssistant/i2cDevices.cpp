/**
 * @file      i2cDevices.cpp
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-02-22
 *
 */
#include "devices.h"

SensorPCF8563 rtc;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C *u8g2 = nullptr;


void setupI2CDevices()
{
#ifdef RTC_IRQ
    pinMode(RTC_IRQ, INPUT_PULLUP);
#endif

    Wire.begin(I2C_SDA, I2C_SCL);

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
        delay(3000);
    }

    if (!rtc.begin(Wire, PCF8563_SLAVE_ADDRESS, I2C_SDA, I2C_SCL)) {
        Serial.println("Failed to find PCF8563 - check your wiring!");
    }



}
















