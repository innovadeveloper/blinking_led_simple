#include <Arduino.h>

#define BUTTON_PIN 15 // D15

int lastState = HIGH;
void setup()
{
  Serial.begin(115200);
  delay(1000);

  // pinMode(BUTTON_PIN, INPUT); // pull-down EXTERNO
  pinMode(BUTTON_PIN, INPUT_PULLUP); // pull-down EXTERNO

  Serial.println("ESP32 listo. Presiona el botón (GPIO 15).");
}

void loop()
{
  int currentState = digitalRead(BUTTON_PIN);


  if (currentState != lastState)
  {
    if (currentState == LOW)
    { 
      Serial.println("[BOTON] PRESIONADO");
    }
    else
    {
      Serial.println("[BOTON] LIBERADO");
    }
    delay(50);
  }

  lastState = currentState;
}

// OPCIÓN 1: Pull-down externo (como tu código actual)
// OPCIÓN 2 (MEJOR): usar pull-up interno del ESP32