#include <listener/button_listener.h>

int lastState = HIGH; // last state because we are using input pull up (intern)
int buttonPinLocally = 0;
int ledPinLocally = 0;
void (*buttonPressedCallback)(bool) = nullptr;

void setupButton(int buttonPin, int ledPin, void (*callback)(bool isPressed))
{
    buttonPinLocally = buttonPin;
    ledPinLocally = ledPin;

    buttonPressedCallback = callback;
    pinMode(buttonPinLocally, INPUT_PULLUP); // pull-down EXTERNO
    pinMode(ledPinLocally, OUTPUT);
}

void onButtonCallback()
{
    int currentState = digitalRead(buttonPinLocally);

    if (currentState != lastState)
    {
        if (currentState == LOW)
        {
            // Serial.println("[BOTON] PRESIONADO");
            digitalWrite(ledPinLocally, HIGH);
            buttonPressedCallback(true);
        }
        else
        {
            buttonPressedCallback(false);
            // Serial.println("[BOTON] LIBERADO");
            digitalWrite(ledPinLocally, LOW);
        }
        delay(50);
    }

    lastState = currentState;
}

// pull down externo vs pull up interno