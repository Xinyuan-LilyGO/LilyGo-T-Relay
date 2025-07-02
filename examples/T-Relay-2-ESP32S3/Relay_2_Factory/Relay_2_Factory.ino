/*
Factory example
*/
#define RELAY_PIN_1 35
#define RELAY_PIN_2 36

#include <AceButton.h>

using namespace ace_button;

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


const uint8_t user_button_pin = 0;
AceButton button(user_button_pin);

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

void handleEvent(AceButton* /* button */, uint8_t eventType,
                 uint8_t buttonState)
{
    // Print out a message for all events.
    bool level = false;
    switch (eventType) {
    case AceButton::kEventClicked:
        level = digitalRead(RELAY_PIN_1);
        digitalWrite(RELAY_PIN_1, !level);
        digitalWrite(RELAY_PIN_2, !level);
        Serial.print("Relay is "); Serial.println(level ? "Open" : "Close");
        break;
    case AceButton::kEventReleased:

        break;
    case AceButton::kEventDoubleClicked:
        digitalWrite(RELAY_PIN_1, LOW);
        digitalWrite(RELAY_PIN_2, LOW);

        digitalWrite(RELAY_PIN_1, HIGH); delay(500);
        digitalWrite(RELAY_PIN_2, HIGH); delay(500);

        digitalWrite(RELAY_PIN_1, LOW); delay(500);
        digitalWrite(RELAY_PIN_2, LOW); delay(500);
        break;
    }
}


void scanWiFi()
{
    // Set WiFi to station mode and disconnect from an AP if it was previously connected.
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    Serial.println("Scan start");
    // WiFi.scanNetworks will return the number of networks found.
    int n = WiFi.scanNetworks();
    Serial.println("Scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.printf("%2d", i + 1);
            Serial.print(" | ");
            Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
            Serial.print(" | ");
            Serial.printf("%4d", WiFi.RSSI(i));
            Serial.print(" | ");
            Serial.printf("%2d", WiFi.channel(i));
            Serial.print(" | ");
            switch (WiFi.encryptionType(i)) {
            case WIFI_AUTH_OPEN:
                Serial.print("open");
                break;
            case WIFI_AUTH_WEP:
                Serial.print("WEP");
                break;
            case WIFI_AUTH_WPA_PSK:
                Serial.print("WPA");
                break;
            case WIFI_AUTH_WPA2_PSK:
                Serial.print("WPA2");
                break;
            case WIFI_AUTH_WPA_WPA2_PSK:
                Serial.print("WPA+WPA2");
                break;
            case WIFI_AUTH_WPA2_ENTERPRISE:
                Serial.print("WPA2-EAP");
                break;
            case WIFI_AUTH_WPA3_PSK:
                Serial.print("WPA3");
                break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
                Serial.print("WPA2+WPA3");
                break;
            case WIFI_AUTH_WAPI_PSK:
                Serial.print("WAPI");
                break;
            default:
                Serial.print("unknown");
            }
            Serial.println();
            delay(10);
        }
    }
    Serial.println("");

    // Delete the scan result to free memory for code below.
    WiFi.scanDelete();
}

void setup()
{
    Serial.begin(115200);

    delay(1000);


    pinMode(user_button_pin, INPUT_PULLUP);
    ButtonConfig* buttonConfig = button.getButtonConfig();
    buttonConfig->setEventHandler(handleEvent);
    buttonConfig->setFeature(ButtonConfig::kFeatureClick);
    buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);

    pinMode(RELAY_PIN_1, OUTPUT);
    pinMode(RELAY_PIN_2, OUTPUT);
    digitalWrite(RELAY_PIN_1, LOW);
    digitalWrite(RELAY_PIN_2, LOW);

    scanWiFi();

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
    Serial.print("SoftApName:"); Serial.println(ssid);
    Serial.print("Password:"); Serial.println(password == NULL ? "No password" : password);
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
    button.check();
}
