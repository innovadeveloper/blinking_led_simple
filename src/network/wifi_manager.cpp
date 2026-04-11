#include "wifi_manager.h"

// const char* ssid = "OnePlus12";
// const char* password = "123456789a";

// evento WiFi (privado a este archivo)
static void WiFiEvent(WiFiEvent_t event) {
  switch(event) {
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

void initWiFiEvents() {
  WiFi.onEvent(WiFiEvent);
}

void connectWiFi(const char* ssid, const char* password) {
  Serial.printf("Connecting to WiFi %s\n", ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}