/*
 * @Description: HT74HC595 Usage Example
 * @Author: LILYGO_L
 * @Date: 2023-08-25 17:09:20
 * @LastEditTime: 2024-07-19 09:49:40
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

    Serial.println("Power on and set the 74HC595 to output disable mode");
    pinMode(HT74HC595_OUT_EN, OUTPUT);
    digitalWrite(HT74HC595_OUT_EN, HIGH); // 关闭输出数据 HT74HC595

    Serial.println("Set all relay outputs to low level");
    HT74HC595->setAllLow();

    Serial.println("Set GPIO4 to low level to enable relay output");
    digitalWrite(HT74HC595_OUT_EN, LOW);
}

void loop()
{
    Serial.printf("System running time: %d\n\n", (uint32_t)millis() / 1000);

    Serial.println("All Channel turn on.");
    HT74HC595->setAllHigh();
    delay(1000);
    Serial.println("All Channel turn off.");
    HT74HC595->setAllLow();
    delay(1000);
    Serial.println("Relay(CH1) turn on");
    HT74HC595->set(0, HIGH, true);
    delay(1000);
    Serial.println("Relay(CH2) turn on");
    HT74HC595->set(0, LOW, true);
    HT74HC595->set(1, HIGH, true);
    delay(1000);
    Serial.println("Relay(CH3) turn on");
    HT74HC595->set(1, LOW, true);
    HT74HC595->set(2, HIGH, true);
    delay(1000);
    Serial.println("Relay(CH4) turn on");
    HT74HC595->set(2, LOW, true);
    HT74HC595->set(3, HIGH, true);
    delay(1000);
    Serial.println("Relay(CH5) turn on");
    HT74HC595->set(3, LOW, true);
    HT74HC595->set(4, HIGH, true);
    delay(1000);
    Serial.println("Relay(CH6) turn on");
    HT74HC595->set(4, LOW, true);
    HT74HC595->set(5, HIGH, true);
    delay(1000);
    Serial.println("LED (CH7) turn on");
    HT74HC595->set(5, LOW, true);
    HT74HC595->set(6, HIGH, true);
    delay(1000);
    Serial.println("LED (CH8) turn on");
    HT74HC595->set(6, LOW, true);
    HT74HC595->set(7, HIGH, true);
    delay(1000);
}
