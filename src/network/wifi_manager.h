#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

// funciones públicas
void connectWiFi(const char* ssid, const char* password);
void initWiFiEvents();

#endif