#include <client/udp_client.h>

WiFiUDP udp;
const char *udpAddressLocally = "0.0.0.0"; // servidor destino
int udpPortLocally = 0;

void setupUDPClient(const char *udpAddress, const int udpPort)
{
    udpAddressLocally = udpAddress;
    udpPortLocally = udpPort;
}

void sendUDP(String message)
{
    udp.beginPacket(udpAddressLocally, udpPortLocally);
    udp.println(message);
    udp.endPacket();
}