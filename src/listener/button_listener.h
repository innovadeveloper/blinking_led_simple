#include <Arduino.h>


#ifndef BUTTON_LISTENER_H
#define BUTTON_LISTENER_H

void setupButton(int buttonPin, int ledPin, void (*callback)(bool isPressed));
void onButtonCallback();
#endif

