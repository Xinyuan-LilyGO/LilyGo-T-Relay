#pragma once
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include <ShiftRegister74HC595_NonTemplate.h>
#include <U8g2lib.h>
#include "SensorPCF8563.tpp"
#include "font/Open_Sans_Hebrew_Bold_32.h"
#include "font/Open_Sans_Hebrew_Condensed_Bold_22.h"
#include <Ticker.h>


struct Settings {
    int  serverPort;            //MQTT PORT
    char deviceName[20];        //DEVICE NAME
    char ipAddress[20];         //MQTT IPADDRESS
    char username[64];          //MQTT USERNAME
    char password[64];          //MQTT PASSWORD
    char tzone[64];             //NTP TZ
    uint16_t switchNum;
    uint16_t shilfNum;
    const char **switchName;     //Switch Name
} ;

extern struct Settings  setting;
extern ShiftRegister74HC595_NonTemplate     *control;
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C  *u8g2;
extern SensorPCF8563                        rtc;
extern Ticker ledTick;

#define DEFAULT_CONFIG_NAME         "/config.json"
#define SETUP_PIN                   0

#undef  I2C_SDA
#undef  I2C_SCL

#define I2C_SDA                     16
#define I2C_SCL                     17
#define RTC_IRQ                     15
#define DATA_PIN                    7
#define CLOCK_PIN                   5
#define LATCH_PIN                   6
#define GREEN_LED_CH                6
#define RED_LED_CH                  7









