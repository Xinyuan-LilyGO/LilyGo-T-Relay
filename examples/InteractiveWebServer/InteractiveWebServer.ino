/*
Interactive example

You need to write the right wifiCredentials.View the IP Address on the serial port.
Enter the IP Address in the same LAN.You can control the state of the relay on the page.

*/
#define RELAY_PIN_1 21
#define RELAY_PIN_2 19
#define RELAY_PIN_3 18
#define RELAY_PIN_4 5
#define LED_PIN     25


#include <Arduino.h>
/* ESP32 Dependencies */
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>


/* Your WiFi Credentials */
const char* ssid = "YourNetworkName"; // SSID
const char* password = "YourPassword"; // Password

/* Start Webserver */
AsyncWebServer server(80);

/* Attach ESP-DASH to AsyncWebServer */
ESPDash dashboard(&server); 

/* 
  Button Card
  Format - (Dashboard Instance, Card Type, Card Name)
*/
Card Relay_1(&dashboard, BUTTON_CARD, "Test Relay_1");
Card Relay_2(&dashboard, BUTTON_CARD, "Test Relay_2");
Card Relay_3(&dashboard, BUTTON_CARD, "Test Relay_3");
Card Relay_4(&dashboard, BUTTON_CARD, "Test Relay_4");

void setup() {
  Serial.begin(115200);


    // Onboard LED light, it can be used freely
  pinMode(RELAY_PIN_1, OUTPUT);
	pinMode(RELAY_PIN_2, OUTPUT);
	pinMode(RELAY_PIN_3, OUTPUT);
	pinMode(RELAY_PIN_4, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
	delay(100);
 
  digitalWrite(RELAY_PIN_1, LOW);
  digitalWrite(RELAY_PIN_2, LOW);
  digitalWrite(RELAY_PIN_3, LOW);
  digitalWrite(RELAY_PIN_4, LOW);
  digitalWrite(LED_PIN, LOW);



  /* Connect WiFi */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.printf("WiFi Failed!\n");
      return;
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  /* Attach Relay_1 Callback */
  Relay_1.attachCallback([&](bool value){
    /* Print our new button value received from dashboard */
    Serial.println("Relay_1 Triggered: "+String((value)?"true":"false"));
    digitalWrite(RELAY_PIN_1, value);
    /* Make sure we update our button's value and send update to dashboard */
    Relay_1.update(value);
    dashboard.sendUpdates();
  });

  /* Attach Relay_2 Callback */
  Relay_2.attachCallback([&](bool value){
    /* Print our new button value received from dashboard */
    Serial.println("Relay_2 Triggered: "+String((value)?"true":"false"));
    digitalWrite(RELAY_PIN_2, value);
    /* Make sure we update our button's value and send update to dashboard */
    Relay_2.update(value);
    dashboard.sendUpdates();
  });

    /* Attach Button Callback */
  Relay_3.attachCallback([&](bool value){
    /* Print our new button value received from dashboard */
    Serial.println("Relay_3 Triggered: "+String((value)?"true":"false"));
    digitalWrite(RELAY_PIN_3, value);
    /* Make sure we update our button's value and send update to dashboard */
    Relay_3.update(value);
    dashboard.sendUpdates();
  });

    /* Attach Button Callback */
  Relay_4.attachCallback([&](bool value){
    /* Print our new button value received from dashboard */
    Serial.println("Relay_4 Triggered: "+String((value)?"true":"false"));
    digitalWrite(RELAY_PIN_4, value);
    /* Make sure we update our button's value and send update to dashboard */
    Relay_4.update(value);
    dashboard.sendUpdates();
  });

  /* Start AsyncWebServer */
  server.begin();
}

void loop() {
  /* Nothing so far */
}
