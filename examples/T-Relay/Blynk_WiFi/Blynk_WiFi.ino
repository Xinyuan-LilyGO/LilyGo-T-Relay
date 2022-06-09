/*************************************************************
Blynk example

You should get Auth Token in the Blynk App.
You need to write the right wifiCredentials.

 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

void setup()
{
    // Debug console
    Serial.begin(115200);

    Blynk.begin(auth, ssid, pass);
}

void loop()
{
    Blynk.run();
}
