
#include <Arduino.h>

#define RELAY_PIN_1 35
#define RELAY_PIN_2 36


void setup()
{
    Serial.begin(115200);
    delay(10);
    Serial.println("setup");

    pinMode(RELAY_PIN_1, OUTPUT);
    pinMode(RELAY_PIN_2, OUTPUT);

}

void loop()
{
    //Turn the relays on and off in turn
    Serial.println("Turn on");
    digitalWrite(RELAY_PIN_1, HIGH);
    delay(1000);
    digitalWrite(RELAY_PIN_2, HIGH);
    delay(1000);

    Serial.println("Turn off");
    digitalWrite(RELAY_PIN_1, LOW);
    delay(1000);
    digitalWrite(RELAY_PIN_2, LOW);
    delay(1000);
}
