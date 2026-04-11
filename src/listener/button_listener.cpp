#include <listener/button_listener.h>

int lastState = HIGH; // last state because we are using input pull up (intern)
int buttonPinLocally = 0;
int ledPinLocally = 0;

void setupButton(int buttonPin, int ledPin)
{
    buttonPinLocally = buttonPin;
    ledPinLocally = ledPin;

    pinMode(buttonPinLocally, INPUT_PULLUP); // pull-down EXTERNO
    pinMode(ledPinLocally, OUTPUT);
}

void onButtonCallback(void (*callback)(bool isPressed))
{
    int currentState = digitalRead(buttonPinLocally);

    if (currentState != lastState)
    {
        if (currentState == LOW)
        {
            // Serial.println("[BOTON] PRESIONADO");
            digitalWrite(ledPinLocally, HIGH);
            callback(true);
        }
        else
        {
            callback(false);
            // Serial.println("[BOTON] LIBERADO");
            digitalWrite(ledPinLocally, LOW);
        }
        delay(50);
    }

    lastState = currentState;
}

// pull down externo vs pull up interno