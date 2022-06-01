
#define RELAY_PIN_1 21
#define RELAY_PIN_2 19
#define RELAY_PIN_3 18
#define RELAY_PIN_4 5
#define LED_PIN     25


void setup()
{
    Serial.begin(115200);
    delay(10);
    Serial.println("setup");

    pinMode(RELAY_PIN_1, OUTPUT);
    pinMode(RELAY_PIN_2, OUTPUT);
    pinMode(RELAY_PIN_3, OUTPUT);
    pinMode(RELAY_PIN_4, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    delay(100);

    //Turn off all relays
    digitalWrite(RELAY_PIN_1, LOW);
    digitalWrite(RELAY_PIN_2, LOW);
    digitalWrite(RELAY_PIN_3, LOW);
    digitalWrite(RELAY_PIN_4, LOW);
    digitalWrite(LED_PIN, LOW);


//Turn the relays on and off in turn
    digitalWrite(RELAY_PIN_1, HIGH);
    delay(500);
    digitalWrite(RELAY_PIN_2, HIGH);
    delay(500);
    digitalWrite(RELAY_PIN_3, HIGH);
    delay(500);
    digitalWrite(RELAY_PIN_4, HIGH);
    delay(500);

    digitalWrite(RELAY_PIN_1, LOW);
    delay(500);
    digitalWrite(RELAY_PIN_2, LOW);
    delay(500);
    digitalWrite(RELAY_PIN_3, LOW);
    delay(500);
    digitalWrite(RELAY_PIN_4, LOW);
    delay(500);


}

void loop()
{

}
