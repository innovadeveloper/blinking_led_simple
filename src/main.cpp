#include <Arduino.h>
#include <network/wifi_manager.h>
#include <listener/button_listener.h>
#include <client/udp_client.h>


#define BUTTON_PIN 15 // D15
#define LED_PIN 2

// const char* ssid = "SAGITARIO";
// const char* password = "5461T4R10";
const char* ssid = "OnePlus12";
const char* password = "123456789a";

const char* udpAddress = "190.102.144.223";
int portAddress = 2028;

void onButtonChangeState(bool isPressed) {
  Serial.print("[BOTON] PRESSED ");
  Serial.println(isPressed);

  // remember that 'String' is not efficient...
  String msg = "pressed=" + String(isPressed);
  sendUDP(msg);
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  setupButton(BUTTON_PIN, LED_PIN);

  initWiFiEvents();
  connectWiFi(ssid, password);

  setupUDPClient(udpAddress, portAddress);
}

void loop()
{
  onButtonCallback(onButtonChangeState);
}

// pull down externo vs pull up interno