/*
 * @Description: 
            T-Relay6 factory test file
        After installing the hardware, open the serial port to obtain the IP and control the relay
 * @Author: LILYGO_L
 * @Date: 2023-07-18 16:27:34
 * @LastEditTime: 2024-07-19 09:53:32
 * @License: GPL 3.0
 */
#include <SPI.h>
#include <Ethernet.h>
#include "ShiftRegister74HC595_NonTemplate.h"
#include "pin_config.h"
#include <iostream>
#include <memory>
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

std::shared_ptr<ShiftRegister74HC595_NonTemplate> HT74HC595 =
    std::make_shared<ShiftRegister74HC595_NonTemplate>(8, HT74HC595_DATA,
                                                       HT74HC595_CLOCK, HT74HC595_LATCH);

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

    pinMode(HT74HC595_OUT_EN, OUTPUT);
    digitalWrite(HT74HC595_OUT_EN, HIGH); // 关闭输出数据 HT74HC595

    // Turn off all relays
    HT74HC595->setAllLow();

    digitalWrite(HT74HC595_OUT_EN, LOW);

    SPI.begin(W5500_SCLK, W5500_MISO, W5500_MOSI, W5500_CS);
    // SPI.setFrequency(80000000);
    EthernetReset(W5500_RST);
    Ethernet.init(W5500_CS);

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

    Serial.end();
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
                        client.println("</head><body><div align=\"center\"><h2>T-Relay6-S3-W5500 Ethernet Server Test</h2><br />");
                        client.print("<h3>Click <a href=\"/Relay_1\">here</a> to change the Relay 1 state.<br></h3>");
                        client.print("<h3>Click <a href=\"/Relay_2\">here</a> to change the Relay 2 state.<br></h3>");
                        client.print("<h3>Click <a href=\"/Relay_3\">here</a> to change the Relay 3 state.<br></h3>");
                        client.print("<h3>Click <a href=\"/Relay_4\">here</a> to change the Relay 4 state.<br></h3>");
                        client.print("<h3>Click <a href=\"/Relay_5\">here</a> to change the Relay 5 state.<br></h3>");
                        client.print("<h3>Click <a href=\"/Relay_6\">here</a> to change the Relay 6 state.<br></h3>");
                        client.print("<h3>Click <a href=\"/Relay_7\">here</a> to change the LED 1 state.<br></h3>");
                        client.print("<h3>Click <a href=\"/Relay_8\">here</a> to change the LED 2 state.<br></h3>");
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
                        HT74HC595->set(0, LOW, true);
                        break;
                    case 1:
                        HT74HC595->set(0, HIGH, true);
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
                        HT74HC595->set(1, LOW, true);
                        break;
                    case 1:
                        HT74HC595->set(1, HIGH, true);
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
                        HT74HC595->set(2, LOW, true);
                        break;
                    case 1:
                        HT74HC595->set(2, HIGH, true);
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
                        HT74HC595->set(3, LOW, true);
                        break;
                    case 1:
                        HT74HC595->set(3, HIGH, true);
                        break;

                    default:
                        break;
                    }
                }
                if (currentLine.endsWith("GET /Relay_5"))
                {
                    HTML_Relay1_Flag = !HTML_Relay1_Flag;
                    switch (HTML_Relay1_Flag)
                    {
                    case 0:
                        HT74HC595->set(4, LOW, true);
                        break;
                    case 1:
                        HT74HC595->set(4, HIGH, true);
                        break;

                    default:
                        break;
                    }
                }
                if (currentLine.endsWith("GET /Relay_6"))
                {
                    HTML_Relay2_Flag = !HTML_Relay2_Flag;
                    switch (HTML_Relay2_Flag)
                    {
                    case 0:
                        HT74HC595->set(5, LOW, true);
                        break;
                    case 1:
                        HT74HC595->set(5, HIGH, true);
                        break;

                    default:
                        break;
                    }
                }
                if (currentLine.endsWith("GET /Relay_7"))
                {
                    HTML_Relay3_Flag = !HTML_Relay3_Flag;
                    switch (HTML_Relay3_Flag)
                    {
                    case 0:
                        HT74HC595->set(6, LOW, true);
                        break;
                    case 1:
                        HT74HC595->set(6, HIGH, true);
                        break;

                    default:
                        break;
                    }
                }
                if (currentLine.endsWith("GET /Relay_8"))
                {
                    HTML_Relay4_Flag = !HTML_Relay4_Flag;
                    switch (HTML_Relay4_Flag)
                    {
                    case 0:
                        HT74HC595->set(7, LOW, true);
                        break;
                    case 1:
                        HT74HC595->set(7, HIGH, true);
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
