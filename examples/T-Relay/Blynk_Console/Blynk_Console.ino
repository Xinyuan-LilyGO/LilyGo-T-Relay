/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest
  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.
    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app
  Blynk library is licensed under MIT license
  This example code is in public domain.
 *************************************************************
  This example runs directly on ESP32 chip.
  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32
  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!
  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#define RELAY_PIN_1 21
#define RELAY_PIN_2 19
#define RELAY_PIN_3 18
#define RELAY_PIN_4 5
#define LED_PIN     25


/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";



BLYNK_WRITE(V0)
{
    if (param.asInt() == 1) {
        digitalWrite(RELAY_PIN_1, HIGH);
        Blynk.logEvent("relay_state", "RELAY_1 ON");//Sending Events
    } else {
        digitalWrite(RELAY_PIN_1,  LOW);
        Blynk.logEvent("relay_state", "RELAY_1 OFF");//Sending Events
    }
}

BLYNK_WRITE(V1)
{
    if (param.asInt() == 1) {
        digitalWrite(RELAY_PIN_2, HIGH);
        Blynk.logEvent("relay_state", "RELAY_2 ON");//Sending Events
    } else {
        digitalWrite(RELAY_PIN_2,  LOW);
        Blynk.logEvent("relay_state", "RELAY_2 OFF");//Sending Events
    }
}

BLYNK_WRITE(V2)
{
    if (param.asInt() == 1) {
        digitalWrite(RELAY_PIN_3, HIGH);
        Blynk.logEvent("relay_state", "RELAY_3 ON");//Sending Events
    } else {
        digitalWrite(RELAY_PIN_3, LOW);
        Blynk.logEvent("relay_state", "RELAY_3 OFF");//Sending Events
    }
}

BLYNK_WRITE(V3)
{
    if (param.asInt() == 1) {
        digitalWrite(RELAY_PIN_4, HIGH );
        Blynk.logEvent("relay_state", "RELAY_4 ON");//Sending Events
    } else {
        digitalWrite(RELAY_PIN_4, LOW);
        Blynk.logEvent("relay_state", "RELAY_4 OFF");//Sending Events
    }
}

//Syncing the output state with the app at startup
BLYNK_CONNECTED()
{
    Blynk.syncVirtual(V0);  // will cause BLYNK_WRITE(V0) to be executed
    Blynk.syncVirtual(V1);  // will cause BLYNK_WRITE(V1) to be executed
    Blynk.syncVirtual(V2);  // will cause BLYNK_WRITE(V2) to be executed
    Blynk.syncVirtual(V3);  // will cause BLYNK_WRITE(V3) to be executed
}

void setup()
{
    // Debug console
    Serial.begin(115200);

    pinMode(RELAY_PIN_1, OUTPUT);
    pinMode(RELAY_PIN_2, OUTPUT);
    pinMode(RELAY_PIN_3, OUTPUT);
    pinMode(RELAY_PIN_4, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    delay(100);

    Blynk.begin(auth, ssid, pass);
}

void loop()
{
    Blynk.run();
}