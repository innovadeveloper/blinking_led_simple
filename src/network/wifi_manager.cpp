#include "wifi_manager.h"

// const char* ssid = "OnePlus12";
// const char* password = "123456789a";

// solo para testings internos..
void traceNetworks()
{
    WiFi.mode(WIFI_STA); 
    WiFi.disconnect();   
    delay(1000);

    int n = WiFi.scanNetworks();

    Serial.println("Escaneo terminado");

    if (n == 0)
    {
        Serial.println("No se encontraron redes");
    }
    else
    {
        Serial.print(n);
        Serial.println(" redes encontradas:");

        for (int i = 0; i < n; ++i)
        {
            Serial.print("SSID: ");
            Serial.print(WiFi.SSID(i));

            Serial.print(" | RSSI: ");
            Serial.print(WiFi.RSSI(i));

            Serial.print(" | Canal: ");
            Serial.print(WiFi.channel(i));

            Serial.print(" | Seguridad: ");
            Serial.println(WiFi.encryptionType(i));

            delay(10);
        }
    }
}

// evento WiFi (privado a este archivo)
static void WiFiEvent(WiFiEvent_t event)
{
    switch (event)
    {
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        Serial.println("WiFi conectado");
        break;

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        Serial.println("WiFi desconectado");
        break;

    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
        break;

    default:
        break;
    }
}

void initWiFiEvents()
{
    WiFi.onEvent(WiFiEvent);
}

void connectWiFi(const char *ssid, const char *password)
{
    Serial.printf("Connecting to WiFi %s\n", ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        // traceNetworks();
    }

    Serial.println("\nConectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}