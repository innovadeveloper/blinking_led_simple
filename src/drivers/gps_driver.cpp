#include <drivers/gps_driver.h>

TinyGPSPlus gps;
HardwareSerial SerialGPS(2); // use UART2 of 3 UART's
unsigned long lastSend = 0;
void (*locationCallback)(String) = nullptr;
int trackIntervalLocally = 0;

void setupGPSDriver(unsigned long baud, int8_t rxPin, int8_t txPin, const int trackInterval,
                    void (*callback)(String payload))
{
    locationCallback = callback;
    trackIntervalLocally = trackInterval;
    SerialGPS.begin(baud, SERIAL_8N1, rxPin, txPin);
}

// solo bloquea si es q hay data disponible dentro del modem gps, y ese bloqueo crecerá segun el buffer...
void readLocationAvailable()
{
    // while (SerialGPS.available())
    // {
    //     char c = SerialGPS.read();
    //     Serial.write(c);  //  imprime NMEA crudo
    // }

    while (SerialGPS.available())
    {
        gps.encode(SerialGPS.read());
    }

    if (millis() - lastSend > trackIntervalLocally) // cada N segundos
    {
        lastSend = millis();

        if (gps.location.isValid())
        {
            String msg = "";

            msg += "lat=" + String(gps.location.lat(), 6);
            msg += ",lng=" + String(gps.location.lng(), 6);
            msg += ",sat=" + String(gps.satellites.value());
            msg += ",hdop=" + String(gps.hdop.hdop());
            msg += ",speed=" + String(gps.speed.kmph());
            msg += ",time=" + String(gps.time.value());
            // Serial.println(msg);
            locationCallback(msg);
        }
        else
        {
            // locationCallback("status=no_fix");
        }
    }
}


    