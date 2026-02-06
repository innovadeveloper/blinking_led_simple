#include <Arduino.h>

#define BUTTON_PIN 15   // D15

int lastState = LOW;

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(BUTTON_PIN, INPUT); // pull-down EXTERNO

  Serial.println("ESP32 listo. Presiona el botón (GPIO 15).");
}

void loop() {
  int currentState = digitalRead(BUTTON_PIN);

  if (currentState != lastState) {
    if (currentState == HIGH) {
      Serial.println("[BOTON] PRESIONADO (HIGH)");
    } else {
      Serial.println("[BOTON] LIBERADO (LOW)");
    }
    delay(50); // debounce simple
  }

  lastState = currentState;
}
