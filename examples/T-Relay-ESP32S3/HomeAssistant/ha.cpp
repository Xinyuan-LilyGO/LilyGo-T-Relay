/**
 * @file      ha.cpp
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-02-22
 *
 */

#include "devices.h"
#include <ArduinoHA.h>
#include <WiFi.h>

WiFiClient  client;
HADevice    *myDevice;
HAMqtt      *mqtt;
HASwitch    *switchPtr;
uint32_t     runningMillis;

void onSwitchCommand(bool state, HASwitch *sender)
{
    String name = sender->getName();

    // Skip the length of MAC + Switch string prefix, and only get the following number as the index
    // "3485188E6834Switch0"
    uint8_t index = name.substring(18).toInt();
    Serial.println("=================");
    Serial.print(name); Serial.print(" ");
    Serial.println(index);
    Serial.println("=================");


    if (control) {
        uint16_t board = index / 6 + 1;
        index = index + ((board - 1) * 2) ;
        Serial.printf("Control relay number = [%d]\n", index);

        if (WiFi.isConnected()) {
            // WiFi is connected, and the green indicator is always on
            control->set(GREEN_LED_CH, HIGH);
        }
        // Keep the power indicator on
        control->set(RED_LED_CH, HIGH);
        state ? control->set(index, HIGH) : control->set(index, LOW);
    }
    sender->setState(state); // report state back to the Home Assistant
}

void onMqttConnected()
{
    Serial.println("Connected to the broker!");
    if (u8g2) {
        u8g2->setDrawColor(0);
        u8g2->drawBox(0, 46, u8g2->getWidth(), 10);
        u8g2->setDrawColor(1);
        u8g2->setFont(u8g2_font_timR08_tr);
        u8g2->drawStr(0, 54, "Connected.");
        u8g2->sendBuffer();
    }
}

bool setupHA(const char *ipaddress,
             const uint16_t port,
             const char *username,
             const char *password)
{
    IPAddress ip;
    if (ip.fromString(ipaddress)) {
        Serial.print("IP param: ");
        Serial.println(ip);
    } else {
        Serial.println("Incorrect IP");
        return false;
    }

    uint8_t mac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    WiFi.macAddress(mac);
    myDevice = new HADevice(mac, sizeof(mac));
    mqtt = new HAMqtt(client, *myDevice, setting.switchNum + 1);
    for (int i = 0; i <= setting.switchNum; ++i) {
        String devs = WiFi.macAddress() + "Switch" + String(i);
        devs.replace(":", "");
        Serial.print("Create "); Serial.print(devs); Serial.println(" Object!");
        switchPtr = new HASwitch(devs.c_str());
        switchPtr->setName(devs.c_str());
        switchPtr->setIcon("mdi:lightbulb");
        switchPtr->onCommand(onSwitchCommand);
    }
    mqtt->onConnected(onMqttConnected);
    mqtt->begin(ip, port, username, password);

    runningMillis = millis();

    return true;
}

void loopHA()
{
    if (!mqtt->isConnected()) {
        if (runningMillis - millis() > 30000UL) {
            runningMillis = millis();
            if (u8g2) {
                u8g2->setDrawColor(0);
                u8g2->drawBox(0, 46, u8g2->getWidth(), 10);
                u8g2->setDrawColor(1);
                u8g2->setFont(u8g2_font_timR08_tr);
                u8g2->drawStr(0, 54, "Connect failed!!");
                u8g2->sendBuffer();
            }
        }
    }
    mqtt->loop();
}

























