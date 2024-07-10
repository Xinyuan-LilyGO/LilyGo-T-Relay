/*
 * @Description:
 * @version: V1.0.0
 * @Author: LILYGO_L
 * @Date: 2023-08-25 17:09:20
 * @LastEditors: LILYGO_L
 * @LastEditTime: 2024-03-30 13:49:38
 * @License: GPL 3.0
 */
#include "ShiftRegister74HC595_NonTemplate.h"
#include "pin_config.h"
#include <iostream>
#include <memory>

std::shared_ptr<ShiftRegister74HC595_NonTemplate> HT74HC595 =
    std::make_shared<ShiftRegister74HC595_NonTemplate>(8, HT74HC595_DATA,
                                                       HT74HC595_CLOCK, HT74HC595_LATCH);

void setup()
{
    Serial.begin(115200);
    Serial.println("Ciallo");

    HT74HC595->setAllLow();
}

void loop()
{
    Serial.printf("System running time: %d\n\n", (uint32_t)millis() / 1000);

    HT74HC595->setAllHigh();
    delay(1000);
    HT74HC595->setAllLow();
    delay(1000);
    HT74HC595->set(0, HIGH, true);
    delay(1000);
    HT74HC595->set(0, LOW, true);
    HT74HC595->set(1, HIGH, true);
    delay(1000);
    HT74HC595->set(1, LOW, true);
    HT74HC595->set(2, HIGH, true);
    delay(1000);
    HT74HC595->set(2, LOW, true);
    HT74HC595->set(3, HIGH, true);
    delay(1000);
    HT74HC595->set(3, LOW, true);
    HT74HC595->set(4, HIGH, true);
    delay(1000);
    HT74HC595->set(4, LOW, true);
    HT74HC595->set(5, HIGH, true);
    delay(1000);
    HT74HC595->set(5, LOW, true);
    HT74HC595->set(6, HIGH, true);
    delay(1000);
    HT74HC595->set(6, LOW, true);
    HT74HC595->set(7, HIGH, true);
    delay(1000);
}
