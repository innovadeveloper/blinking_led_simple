#include <Arduino.h>

#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

#include <network/wifi_manager.h>
#include <listener/button_listener.h>
#include <client/udp_client.h>
#include <client/http_client.h>
#include <drivers/gps_driver.h>

#define BUTTON_PIN 15 // D15
#define LED_PIN 2

#define LED_001_PIN 2
#define LED_002_PIN 4

#define RX2_PIN 16
#define TX2_PIN 17
#define TRACK_INTERVAL 6000 // x ms

// const char *ssid = "SAGITARIO";
// const char *password = "5461T4R10";
// const char *ssid = "FnC";
// const char *password = "12345678";
const char *ssid = "OnePlus12";
const char *password = "123456789a";

const char *udpAddress = "178.156.133.161";
int portAddress = 9007;

void onButtonChangeState(bool isPressed)
{
  // Serial.print("[BOTON] PRESSED ");
  // Serial.println(isPressed);

  // String accessToken = recoveryAccessToken();

  // // remember that 'String' is not efficient...
  // String msg = "pressed=" + String(isPressed) + ",access_token=" + String(accessToken);
  // sendUDP(msg);

  if(isPressed){
    // led 001
    digitalWrite(LED_001_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_001_PIN, LOW);


    // led 002
    digitalWrite(LED_002_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_002_PIN, LOW);
  }
}

// lat=-11.995333,lng=-77.058607,sat=8,hdop=1.10,speed=0.28,time=12393700
void onLocationAvailable(String payload)
{
  Serial.println(payload);
  sendUDP(payload);
}


void setup()
{
  Serial.begin(115200);
  delay(1000);


  initWiFiEvents();
  connectWiFi(ssid, password);

  setupButton(BUTTON_PIN, LED_PIN, onButtonChangeState);
  setupUDPClient(udpAddress, portAddress);
  setupGPSDriver(9600, RX2_PIN, TX2_PIN, TRACK_INTERVAL, onLocationAvailable);


  // NEW CONFIGURATION EXPRESS
  pinMode(LED_001_PIN, OUTPUT);
  pinMode(LED_002_PIN, OUTPUT);
}

void loop()
{
  onButtonCallback();
  readLocationAvailable();
}

// pull down externo vs pull up interno