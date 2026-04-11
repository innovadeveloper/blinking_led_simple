#include <Arduino.h>


#ifndef BUTTON_LISTENER_H
#define BUTTON_LISTENER_H

void setupButton(int buttonPin, int ledPin);
void onButtonCallback(void (*callback)(bool isPressed));

#endif

