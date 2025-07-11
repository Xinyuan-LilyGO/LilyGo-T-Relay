/**
 * @brief None
 * @return
 * @Date 2025-07-11 16:39:44
 */
#include <ETH.h>
#include <SPI.h>
#include "pin_config.h"

// Set this to 1 to enable dual Ethernet support
#define USE_TWO_ETH_PORTS 0

#ifndef ETH_PHY_TYPE
#define ETH_PHY_TYPE ETH_PHY_W5500
#define ETH_PHY_ADDR 1
#define ETH_PHY_CS PIN_W5500_CS
#define ETH_PHY_IRQ PIN_W5500_INT
#define ETH_PHY_RST PIN_W5500_RESET
#endif

// SPI pins
#define ETH_SPI_SCK PIN_W5500_CLK
#define ETH_SPI_MISO PIN_W5500_MISO
#define ETH_SPI_MOSI PIN_W5500_MOSI

#if USE_TWO_ETH_PORTS
// Second port on shared SPI bus
#ifndef ETH1_PHY_TYPE
#define ETH1_PHY_TYPE ETH_PHY_W5500
#define ETH1_PHY_ADDR 1
#define ETH1_PHY_CS 32
#define ETH1_PHY_IRQ 33
#define ETH1_PHY_RST 18
#endif
ETHClass ETH1(1);
#endif

static bool eth_connected = false;

void onEvent(arduino_event_id_t event, arduino_event_info_t info)
{
    switch (event)
    {
    case ARDUINO_EVENT_ETH_START:
        Serial.println("ETH Started");
        // set eth hostname here
        ETH.setHostname("esp32-eth0");
        break;
    case ARDUINO_EVENT_ETH_CONNECTED:
        Serial.println("ETH Connected");
        break;
    case ARDUINO_EVENT_ETH_GOT_IP:
        Serial.printf("ETH Got IP: '%s'\n", esp_netif_get_desc(info.got_ip.esp_netif));
        Serial.println(ETH);
#if USE_TWO_ETH_PORTS
        Serial.println(ETH1);
#endif
        eth_connected = true;
        break;
    case ARDUINO_EVENT_ETH_LOST_IP:
        Serial.println("ETH Lost IP");
        eth_connected = false;
        break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
        Serial.println("ETH Disconnected");
        eth_connected = false;
        break;
    case ARDUINO_EVENT_ETH_STOP:
        Serial.println("ETH Stopped");
        eth_connected = false;
        break;
    default:
        break;
    }
}

void testClient(const char *host, uint16_t port)
{
    Serial.print("\nconnecting to ");
    Serial.println(host);

    NetworkClient client;
    if (!client.connect(host, port))
    {
        Serial.println("connection failed");
        return;
    }
    client.printf("GET / HTTP/1.1\r\nHost: %s\r\n\r\n", host);
    while (client.connected() && !client.available())
        ;
    while (client.available())
    {
        Serial.write(client.read());
    }

    Serial.println("closing connection\n");
    client.stop();
}

void setup()
{
    Serial.begin(115200);
    Network.onEvent(onEvent);

    SPI.begin(ETH_SPI_SCK, ETH_SPI_MISO, ETH_SPI_MOSI);
    ETH.begin(ETH_PHY_TYPE, ETH_PHY_ADDR, ETH_PHY_CS, ETH_PHY_IRQ, ETH_PHY_RST, SPI);
#if USE_TWO_ETH_PORTS
    ETH1.begin(ETH1_PHY_TYPE, ETH1_PHY_ADDR, ETH1_PHY_CS, ETH1_PHY_IRQ, ETH1_PHY_RST, SPI);
#endif
}

void loop()
{
    if (eth_connected)
    {
        testClient("google.com", 80);
    }
    delay(10000);
}
