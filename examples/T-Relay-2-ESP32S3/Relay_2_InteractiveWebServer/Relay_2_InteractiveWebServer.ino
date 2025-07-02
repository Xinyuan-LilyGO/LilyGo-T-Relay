/*
Interactive example

You need to write the right wifiCredentials.View the IP Address on the serial port.
Enter the IP Address in the same LAN.You can control the state of the relay on the page.

*/
#define RELAY_PIN_1 35
#define RELAY_PIN_2 36


#include <Arduino.h>
/* ESP32 Dependencies */
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>

// #define WIFI_STA_MODE       //Connect to your wifi , open this marco

/* Your WiFi Credentials */
#ifdef WIFI_STA_MODE
const char *ssid = "YourNetworkName"; // SSID
const char *password = "YourPassword"; // Password
#else
// Soft ap config
const char *ssid = "LilyGo-Relay2"; // SSID
const char *password = NULL; // Password , NULL means no password is set 
#endif

/* Start Webserver */
AsyncWebServer server(80);

/* Attach ESP-DASH to AsyncWebServer */
ESPDash dashboard(&server);

/*
  Button Card
  Format - (Dashboard Instance, Card Type, Card Name)
*/
Card Relay_1(&dashboard, BUTTON_CARD, "Relay_1");
Card Relay_2(&dashboard, BUTTON_CARD, "Relay_2");

void setup()
{
    Serial.begin(115200);


    // Onboard LED light, it can be used freely
    pinMode(RELAY_PIN_1, OUTPUT);
    pinMode(RELAY_PIN_2, OUTPUT);
    delay(100);

    digitalWrite(RELAY_PIN_1, LOW);
    digitalWrite(RELAY_PIN_2, LOW);

#ifdef WIFI_STA_MODE
    /* Connect WiFi */
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
#else
    Serial.println("Configuring access point...");

    // You can remove the password parameter if you want the AP to be open.
    // a valid password must have more than 7 characters
    if (!WiFi.softAP(ssid, password)) {
        log_e("Soft AP creation failed.");
        while (1);
    }
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    server.begin();
    Serial.println("Server started");
#endif

    /* Attach Relay_1 Callback */
    Relay_1.attachCallback([&](bool value) {
        /* Print our new button value received from dashboard */
        Serial.println("Relay_1 Triggered: " + String((value) ? "true" : "false"));
        digitalWrite(RELAY_PIN_1, value);
        /* Make sure we update our button's value and send update to dashboard */
        Relay_1.update(value);
        dashboard.sendUpdates();
    });

    /* Attach Relay_2 Callback */
    Relay_2.attachCallback([&](bool value) {
        /* Print our new button value received from dashboard */
        Serial.println("Relay_2 Triggered: " + String((value) ? "true" : "false"));
        digitalWrite(RELAY_PIN_2, value);
        /* Make sure we update our button's value and send update to dashboard */
        Relay_2.update(value);
        dashboard.sendUpdates();
    });


    /* Start AsyncWebServer */
    server.begin();
}

void loop()
{
    /* Nothing so far */
}
