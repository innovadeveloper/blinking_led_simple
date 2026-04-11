#include <Arduino.h>

#define BUTTON_PIN 15 // D15
#define LED_PIN 2

int lastState = HIGH;
void setup()
{
  Serial.begin(115200);
  delay(1000);

  // pinMode(BUTTON_PIN, INPUT); // pull-down EXTERNO
  pinMode(BUTTON_PIN, INPUT_PULLUP); // pull-down EXTERNO
  pinMode(LED_PIN, OUTPUT);

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
      digitalWrite(LED_PIN, HIGH);
    }
    else
    {
      Serial.println("[BOTON] LIBERADO");
      digitalWrite(LED_PIN, LOW);
    }
    delay(50);
  }

  lastState = currentState;
}

// pull down externo vs pull up interno