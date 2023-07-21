/*
 * @Description: LilyGo-T-Relay-4_W5500 Ethernet Server Test
    Instructions for use:
    This procedure is used to test the W5500 Ethernet module with LilyGo-T-Relay-4 extension.
    Connect the W5500 network cable to the router device port, the router will automatically
assign a DHCP dynamic IP to the W5500, if the allocation is successful, the system will print
the [DHCP] IP to the serial port, otherwise a static IP will be assigned.
    Open the browser and enter this IP to switch and control the relay.

 * @version: V1.0.0
 * @Author: LILYGO_L
 * @Date: 2023-07-18 16:27:34
 * @LastEditors: LILYGO_L
 * @LastEditTime: 2023-07-21 15:39:04
 * @License: GPL 3.0
 */
#include <SPI.h>
#include <Ethernet.h>
#include "pin_config.h"
#include "utility/w5100.h"

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
// The MAC address must be an even number first !
uint8_t mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 177);
IPAddress myDns(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);
boolean gotAMessage = false; // whether or not you got a message from the client yet

// Web relay trigger flag
bool HTML_Relay1_Flag = 0;
bool HTML_Relay2_Flag = 0;
bool HTML_Relay3_Flag = 0;
bool HTML_Relay4_Flag = 0;

/**
 * @brief W5500 reset pin delay
 * @param resetPin W5500 reset pin
 * @return
 * @Date 2023-07-19 11:31:23
 */
void EthernetReset(const uint8_t resetPin)
{
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);
    delay(250);
    digitalWrite(resetPin, LOW);
    delay(50);
    digitalWrite(resetPin, HIGH);
    delay(350);
}

void setup()
{
    Serial.begin(115200);

    pinMode(RELAY4_PIN_1, OUTPUT);
    pinMode(RELAY4_PIN_2, OUTPUT);
    pinMode(RELAY4_PIN_3, OUTPUT);
    pinMode(RELAY4_PIN_4, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    delay(100);

    // Turn off all relays
    digitalWrite(RELAY4_PIN_1, LOW);
    digitalWrite(RELAY4_PIN_2, LOW);
    digitalWrite(RELAY4_PIN_3, LOW);
    digitalWrite(RELAY4_PIN_4, LOW);
    digitalWrite(LED_PIN, LOW);

    SPI.begin(PIN_W5500_CLK, PIN_W5500_MISO, PIN_W5500_MOSI, PIN_W5500_CS);
    EthernetReset(PIN_W5500_RESET);
    Ethernet.init(PIN_W5500_CS);

    W5100.init();
    delay(1000); // give the Ethernet shield a second to initialize

    switch (Ethernet.hardwareStatus())
    {
    case EthernetNoHardware:
        // no point in carrying on, so do nothing forevermore:
        while (true)
        {
            Serial.println("Ethernet No Hardware");
            delay(1000);
        }
        break;
    case EthernetW5100:
        Serial.println("Ethernet W5100 Discovery !");
        break;
    case EthernetW5200:
        Serial.println("Ethernet W5200 Discovery !");
        break;
    case EthernetW5500:
        Serial.println("Ethernet W5500 Discovery !");
        break;
    }

    switch (Ethernet.linkStatus())
    {
    case Unknown:
        // no point in carrying on, so do nothing forevermore:
        while (true)
        {
            Serial.print("Link status: ");
            Serial.println("Unknown");
            Serial.println("Hardware error !");
            delay(1000);
        }
        break;
    case LinkON:
        Serial.print("Link status: ");
        Serial.println("ON");
        break;
    case LinkOFF:
        Serial.print("Link status: ");
        Serial.println("OFF");
        Serial.println("The network cable is not connected !");

        while (Ethernet.linkStatus() == LinkOFF)
        {
            Serial.println("Please insert the network cable and try again !");
            delay(1000);
        }
        if (Ethernet.linkStatus() == LinkON)
        {
            Serial.print("Link status: ");
            Serial.println("ON");
        }
        else
        {
            // no point in carrying on, so do nothing forevermore:
            while (true)
            {
                Serial.println("Hardware error !");
                delay(1000);
            }
        }
        break;
    }

    // start the Ethernet connection:
    Serial.println("Trying to get an IP address using DHCP...");
    if (Ethernet.begin(mac) == 0)
    {
        // initialize the Ethernet device not using DHCP:
        Ethernet.begin(mac, ip, myDns, gateway, subnet);

        Serial.println("-------------------------");
        Serial.println("[INFO] Configuring random DHCP failed !");
        Serial.println("");
        Serial.println("[INFO] Configuring static IP...");
        Serial.print("[Static] IP Address: ");
        Serial.println(Ethernet.localIP());
        Serial.print("[Static] Subnet Mask: ");
        Serial.println(Ethernet.subnetMask());
        Serial.print("[Static] Gateway: ");
        Serial.println(Ethernet.gatewayIP());
        Serial.print("[Static] DNS: ");
        Serial.println(Ethernet.dnsServerIP());
        Serial.println("-------------------------");
        Serial.println("");
    }
    else
    {
        Serial.println("-------------------------");
        Serial.println("[INFO] Configuring random DHCP successfully !");
        Serial.println("");
        Serial.print("[DHCP] IP Address: ");
        Serial.println(Ethernet.localIP());
        Serial.print("[DHCP] Subnet Mask: ");
        Serial.println(Ethernet.subnetMask());
        Serial.print("[DHCP] Gateway: ");
        Serial.println(Ethernet.gatewayIP());
        Serial.print("[DHCP] DNS: ");
        Serial.println(Ethernet.dnsServerIP());
        Serial.println("-------------------------");
        Serial.println("");
    }

    // start listening for clients
    server.begin();
}

void loop()
{
    EthernetClient client = server.available();
    if (client)
    {                                  // if you get a client,
        Serial.println("New Client."); // print a message out the serial port
        String currentLine = "";       // make a String to hold incoming data from the client
        while (client.connected())
        { // loop while the client's connected
            if (client.available())
            {                           // if there's bytes to read from the client,
                char c = client.read(); // read a byte, then
                Serial.write(c);        // print it out the serial monitor
                if (c == '\n')
                { // if the byte is a newline character

                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0)
                    {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();

                        // the content of the HTTP response follows the header:
                        client.println("</head><body><div align=\"center\"><h2>LilyGo-T-Relay-4_W5500 Ethernet Server Test</h2><br />");
                        client.print("<h3>Click <a href=\"/Relay_1\">here</a> to change the Relay 1 state.<br></h3>");
                        client.print("<h3>Click <a href=\"/Relay_2\">here</a> to change the Relay 2 state.<br></h3>");
                        client.print("<h3>Click <a href=\"/Relay_3\">here</a> to change the Relay 3 state.<br></h3>");
                        client.print("<h3>Click <a href=\"/Relay_4\">here</a> to change the Relay 4 state.<br></h3>");
                        // The HTTP response ends with another blank line:
                        client.println();
                        // break out of the while loop:
                        break;
                    }
                    else
                    { // if you got a newline, then clear currentLine:
                        currentLine = "";
                    }
                }
                else if (c != '\r')
                {                     // if you got anything else but a carriage return character,
                    currentLine += c; // add it to the end of the currentLine
                }

                // Check to see if the client request was "GET /H" or "GET /L":
                if (currentLine.endsWith("GET /Relay_1"))
                {
                    HTML_Relay1_Flag = !HTML_Relay1_Flag;
                    switch (HTML_Relay1_Flag)
                    {
                    case 0:
                        digitalWrite(RELAY4_PIN_1, LOW);
                        break;
                    case 1:
                        digitalWrite(RELAY4_PIN_1, HIGH);
                        break;

                    default:
                        break;
                    }
                }
                if (currentLine.endsWith("GET /Relay_2"))
                {
                    HTML_Relay2_Flag = !HTML_Relay2_Flag;
                    switch (HTML_Relay2_Flag)
                    {
                    case 0:
                        digitalWrite(RELAY4_PIN_2, LOW);
                        break;
                    case 1:
                        digitalWrite(RELAY4_PIN_2, HIGH);
                        break;

                    default:
                        break;
                    }
                }
                if (currentLine.endsWith("GET /Relay_3"))
                {
                    HTML_Relay3_Flag = !HTML_Relay3_Flag;
                    switch (HTML_Relay3_Flag)
                    {
                    case 0:
                        digitalWrite(RELAY4_PIN_3, LOW);
                        break;
                    case 1:
                        digitalWrite(RELAY4_PIN_3, HIGH);
                        break;

                    default:
                        break;
                    }
                }
                if (currentLine.endsWith("GET /Relay_4"))
                {
                    HTML_Relay4_Flag = !HTML_Relay4_Flag;
                    switch (HTML_Relay4_Flag)
                    {
                    case 0:
                        digitalWrite(RELAY4_PIN_4, LOW);
                        break;
                    case 1:
                        digitalWrite(RELAY4_PIN_4, HIGH);
                        break;

                    default:
                        break;
                    }
                }
            }
        }
        // close the connection:
        client.stop();
        Serial.println("Client Disconnected.");
        // give the web browser time to receive the data
        delay(500);
    }
}
