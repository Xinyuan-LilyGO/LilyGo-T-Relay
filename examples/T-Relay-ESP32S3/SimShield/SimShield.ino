/**
 * @file      SimShield.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2025  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2025-06-28
 * @note      More examples see  https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX
 */
#include <Arduino.h>

#define MODEM_PWRKEY                46
#define MODEM_RX_PIN                9
#define MODEM_TX_PIN                3
#define MODEM_DTR_PIN               11
#define MODEM_RING_PIN              13

#define RELAY_ENABLE                4

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to the module)
#define SerialAT  Serial1

uint32_t AutoBaud()
{
    static uint32_t rates[] = {115200, 9600, 57600,  38400, 19200,  74400, 74880,
                               230400, 460800, 2400,  4800,  14400, 28800
                              };
    for (uint8_t i = 0; i < sizeof(rates) / sizeof(rates[0]); i++) {
        uint32_t rate = rates[i];
        Serial.printf("Trying baud rate %u\n", rate);
        SerialAT.updateBaudRate(rate);
        delay(10);
        for (int j = 0; j < 10; j++) {
            SerialAT.print("AT\r\n");
            String input = SerialAT.readString();
            if (input.indexOf("OK") >= 0) {
                Serial.printf("Modem responded at rate:%u\n", rate);
                return rate;
            }
        }
    }
    SerialAT.updateBaudRate(115200);
    return 0;
}

void setup()
{
    // Set Relay enable pin to output
    pinMode(RELAY_ENABLE, OUTPUT);
    // Relay output enable pin disable
    digitalWrite(RELAY_ENABLE, HIGH);

    // Set console baud rate
    SerialMon.begin(115200);

    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

    Serial.println("Start modem...");

    pinMode(MODEM_PWRKEY, OUTPUT);
    // Pull down PWRKEY for more than 1 second according to manual requirements
    digitalWrite(MODEM_PWRKEY, HIGH);
    delay(100);
    digitalWrite(MODEM_PWRKEY, LOW);
    delay(1000);
    digitalWrite(MODEM_PWRKEY, HIGH);

    delay(10000);

    // DTR pin
    pinMode(MODEM_DTR_PIN, OUTPUT);
    digitalWrite(MODEM_DTR_PIN, LOW);

    // Ring pin
    pinMode(MODEM_RING_PIN, INPUT_PULLUP);

    if (AutoBaud()) {
        Serial.println(F("***********************************************************"));
        Serial.println(F(" You can now send AT commands"));
        Serial.println(F(" Enter \"AT\" (without quotes), and you should see \"OK\""));
        Serial.println(F(" If it doesn't work, select \"Both NL & CR\" in Serial Monitor"));
        Serial.println(F(" DISCLAIMER: Entering AT commands without knowing what they do"));
        Serial.println(F(" can have undesired consequences..."));
        Serial.println(F("More examples see  https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX"));
        Serial.println(F("***********************************************************\n"));
    } else {
        Serial.println(F("***********************************************************"));
        Serial.println(F(" Failed to connect to the modem! Check the baud and try again."));
        Serial.println(F("***********************************************************\n"));
    }

    delay(1000);

    // Check sim-card insert?
    SerialAT.println("AT+CPIN?");

    // Set Modem sleep mode
    // SerialAT.println("AT+CSCLK=1");
    // delay(5000);

    // Pull up DTR to put the modem into sleep
    // Serial.println("Enter modem to sleep mode");
    // digitalWrite(MODEM_DTR_PIN, HIGH);

}

void loop()
{
    if (digitalRead(MODEM_RING_PIN) == LOW) {
        Serial.println("Ring is coming!");
    }

    while (SerialAT.available()) {
        SerialMon.write(SerialAT.read());
    }
    while (SerialMon.available()) {
        SerialAT.write(SerialMon.read());
    }

}
