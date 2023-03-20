/**
 * @file      wm.cpp
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-02-22
 *
 */

#include <WiFiManager.h>            // https://github.com/tzapu/WiFiManager
#include "SPIFFS.h"
#include <FS.h>
#include <ArduinoJson.h>            //https://github.com/bblanchon/ArduinoJson
#include "devices.h"
#include "sntp.h"



bool needConfig = false;
struct Settings  setting;

const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";

void printConfig()
{
    Serial.print("MQTT Borker port:"); Serial.println(setting.serverPort);
    Serial.print("MQTT Borker address:"); Serial.println(setting.ipAddress);
    Serial.print("MQTT Borker username:"); Serial.println(setting.username);
    Serial.print("MQTT Borker password:"); Serial.println(setting.password);
    Serial.print("Config Switch Number:");  Serial.println(setting.switchNum);
    Serial.print("Config NTP TimeZone:");  Serial.println(setting.tzone);

}



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
}

void startLed()
{
    ledTick.attach_ms(500, []() {
        static bool level;
        if (control) {
            control->set(GREEN_LED_CH, level);
        }
        level = !level;
    });
}

void stopLed()
{
    ledTick.detach();
    if (control) {
        control->set(GREEN_LED_CH, LOW);
    }
}


void setupWM()
{



    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

    pinMode(SETUP_PIN, INPUT_PULLUP);
    if (control) {
        control->set(RED_LED_CH, HIGH);
    }
    if (SPIFFS.begin()) {
        Serial.println("mounted file system");
        if (!SPIFFS.exists(DEFAULT_CONFIG_NAME)) {
            needConfig = true;
            setting.switchNum = 6;
            setting.serverPort = 1883;
            strcpy(setting.ipAddress, "192.168.36.11");
            strcpy(setting.tzone, "CST-8");
            strcpy(setting.deviceName, "LilyGoRelay");
        } else {
            //file exists, reading and loading
            Serial.println("reading config file");
            File f = SPIFFS.open(DEFAULT_CONFIG_NAME, "r");
            if (f) {
                Serial.println("opened config file");
                size_t size = f.size();
                // Allocate a buffer to store contents of the file.
                std::unique_ptr<char[]> buf(new char[size]);
                f.readBytes(buf.get(), size);
                DynamicJsonDocument json(1024);
                auto deserializeError = deserializeJson(json, buf.get());
                serializeJson(json, Serial);
                if ( ! deserializeError ) {
                    Serial.println("\nMQTT Borker");
                    setting.serverPort = json["serverPort"];
                    strcpy(setting.ipAddress, json["ipAddress"]);
                    strcpy(setting.username, json["username"]);
                    strcpy(setting.password, json["password"]);
                    strcpy(setting.deviceName, json["deviceName"]);
                    strcpy(setting.tzone, json["tzone"]);
                    setting.switchNum = json["switchNum"];
                    setting.shilfNum = json["shilfNum"];
                    printConfig();
                } else {
                    Serial.println("failed to load json config");
                    needConfig = true;
                }
                f.close();
            }
        }
    } else {
        if (u8g2) {
            u8g2->clear();
            u8g2->setDrawColor(1);
            u8g2->setFont(u8g2_font_timR10_tr);
            u8g2->drawBox(0, 0, u8g2->getWidth(), 14);
            u8g2->setDrawColor(0);
            u8g2->drawStr(0, 12, "Formating...");
            u8g2->sendBuffer();
        }
        if (!SPIFFS.format()) {
            Serial.println("Fatal error, unable to initialize SPIFFS.");
            assert(0);
        }
        SPIFFS.begin();
        needConfig = true;

        //default setting value
        setting.serverPort = 1883;
        strcpy(setting.ipAddress, "192.168.36.11");
        strcpy(setting.deviceName, "LilyGo-T-Relay");
        strcpy(setting.tzone, "CST-8");
        setting.switchNum = 6;
        setting.shilfNum = 1;
        Serial.println("Save default config.");
    }



    if (u8g2) {
        u8g2->clear();
        u8g2->setDrawColor(1);
        u8g2->setFont(u8g2_font_timR10_tr);
        u8g2->drawBox(0, 0, u8g2->getWidth(), 14);
        u8g2->setDrawColor(0);
        u8g2->drawStr(0, 12, "Starting...");
        u8g2->setDrawColor(1);
        u8g2->drawStr(0, 26, "Press and hold the BO ");
        u8g2->drawStr(0, 40, "OT key to enter the conf");
        u8g2->drawStr(0, 54, "iguration mode");
        u8g2->sendBuffer();
    }


    //Delay to push SETUP button
    if (!needConfig) {
        Serial.println("Press setup button");
        for (int sec = 5; sec > 0; sec--) {
            Serial.print(sec);
            Serial.print("..");
            delay(1000);
        }
    }

CONFIG:

    Serial.println("Settings loaded");
    if (digitalRead(SETUP_PIN) == LOW || needConfig) {

        startLed();

        if (u8g2) {
            u8g2->clear();
            u8g2->setDrawColor(1);
            u8g2->drawBox(0, 0, u8g2->getWidth(), 14);
            u8g2->setDrawColor(0);
            u8g2->drawStr(0, 12, "Configure Network");
            u8g2->setDrawColor(1);
            u8g2->drawStr(0, 26, "Please connect \"LiLy");
            u8g2->drawStr(0, 40, "Go-Relay\" hotspot");
            u8g2->drawStr(0, 54, "setting network");
            u8g2->sendBuffer();
        }
        Serial.println("Please connect \"LilyGo-Relay\" wireless hotspot to set network connection parameters");



        printConfig();
        // Button pressed
        Serial.println("SETUP");
        WiFiManager wm;
        setting.deviceName[19] = '\0'; // add null terminator at the end cause overflow
        setting.ipAddress[19] = '\0'; // add null terminator at the end cause overflow
        setting.username[63] = '\0'; // add null terminator at the end cause overflow
        setting.password[63] = '\0'; // add null terminator at the end cause overflow
        setting.tzone[63] = '\0'; // add null terminator at the end cause overflow
        WiFiManagerParameter param_device_name( "1", "Device Name",  setting.deviceName, 20);
        WiFiManagerParameter param_mqtt_server( "2", "MQTT Server",  setting.ipAddress, 20);
        WiFiManagerParameter param_mqtt_username( "3", "MQTT Username",  setting.username, 64);
        WiFiManagerParameter param_mqtt_password( "4", "MQTT Password",  setting.password, 64);
        WiFiManagerParameter param_mqtt_port( "5", "MQTT Port",  String(setting.serverPort).c_str(), 10);
        WiFiManagerParameter param_switch_number( "6", "SwitchNumber",  String(setting.switchNum).c_str(), 10);
        WiFiManagerParameter param_timezone( "7", "TimeZone",  String(setting.tzone).c_str(), 64);

        wm.setDarkMode(true);
        wm.setShowStaticFields(true);
        wm.setTimeout(0);
        wm.setTitle("LilyGo");
        wm.setScanDispPerc(true);
        wm.setMinimumSignalQuality(40);
        wm.setCaptivePortalEnable(false);

        wm.addParameter(&param_device_name );
        wm.addParameter(&param_mqtt_username );
        wm.addParameter(&param_mqtt_password );
        wm.addParameter(&param_mqtt_server );
        wm.addParameter(&param_mqtt_port );
        wm.addParameter(&param_switch_number );
        wm.addParameter(&param_timezone );


        //SSID & password parameters already included
        bool result =  false;
        do {

            // Serial.println("Please check the connection parameters");

        } while (wm.startConfigPortal("LilyGo-Relay", "12345678") == false);

        strncpy(setting.deviceName, param_device_name.getValue(), 20);
        strncpy(setting.ipAddress, param_mqtt_server.getValue(), 20);
        strncpy(setting.username, param_mqtt_username.getValue(), 64);
        strncpy(setting.password, param_mqtt_password.getValue(), 64);
        strncpy(setting.tzone, param_timezone.getValue(), 64);
        setting.serverPort = String(param_mqtt_port.getValue()).toInt();
        setting.switchNum = String(param_switch_number.getValue()).toInt();

        setting.deviceName[19] = '\0';
        setting.ipAddress[19] = '\0';   // add null terminator at the end cause overflow
        setting.username[63] = '\0';    // add null terminator at the end cause overflow
        setting.password[63] = '\0';    // add null terminator at the end cause overflow
        setting.tzone[63] = '\0';    // add null terminator at the end cause overflow

        Serial.print("MQTT Device Name:");      Serial.println(param_device_name.getValue());
        Serial.print("MQTT Server address:");   Serial.println(param_mqtt_server.getValue());
        Serial.print("MQTT Borker port:");      Serial.println(param_mqtt_port.getValue());
        Serial.print("MQTT Borker username:");  Serial.println(param_mqtt_username.getValue());
        Serial.print("MQTT Borker password:");  Serial.println(param_mqtt_password.getValue());
        Serial.print("Config Switch Number:");  Serial.println(param_switch_number.getValue());
        Serial.print("Config NTP TimeZone:");  Serial.println(param_timezone.getValue());


        File f = SPIFFS.open(DEFAULT_CONFIG_NAME, "w", true);
        if (f) {

            DynamicJsonDocument json(512);

            json["deviceName"] = setting.deviceName;
            json["serverPort"] = setting.serverPort;
            json["ipAddress"] = setting.ipAddress;
            json["username"] = setting.username;
            json["password"] = setting.password;

            setting.shilfNum = (setting.switchNum / 6 );

            json["shilfNum"] = setting.shilfNum;
            json["switchNum"] = setting.switchNum;
            json["tzone"] = setting.tzone;

            // Serialize JSON to file
            if (serializeJson(json, f) == 0) {
                Serial.println(F("Failed to write to file"));
            }
            f.close();

        } else {
            Serial.println("Fatal error, unable to write file to SPIFFS.");
            assert(0);
        }

        printConfig();

    } else {
        //connect to saved SSID
        WiFi.begin();
        printConfig();
    }

    // set notification call-back function
    sntp_set_time_sync_notification_cb( timeavailable );

    /**
     * A more convenient approach to handle TimeZones with daylightOffset
     * would be to specify a environmnet variable with TimeZone definition including daylight adjustmnet rules.
     * A list of rules for your zone could be obtained from https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
     */
    configTzTime(setting.tzone, ntpServer1, ntpServer2);

    if (WiFi.waitForConnectResult(30000UL) != WL_CONNECTED) {
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
            goto CONFIG;
        }
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    if (u8g2) {
        u8g2->clear();
        u8g2->setFont(u8g2_font_timR10_tr);
        u8g2->setDrawColor(1);
        u8g2->drawBox(0, 0, u8g2->getWidth(), 14);
        u8g2->setDrawColor(0);
        u8g2->drawStr(0, 12, "Device Info");
        u8g2->setDrawColor(1);
        u8g2->setFont(u8g2_font_timR08_tr);
        u8g2->drawStr(0, 26, (String("BroKer:") + String(setting.ipAddress)).c_str());
        u8g2->drawStr(0, 40, ( String("Addr:") + WiFi.localIP().toString()).c_str());
        u8g2->drawStr(0, 54, "Connecting ....");
        u8g2->sendBuffer();
    }

    stopLed();

    if (control) {
        control->set(GREEN_LED_CH, HIGH);
    }

    Serial.printf("Has Relay Board = %d\n", setting.shilfNum);

}





















