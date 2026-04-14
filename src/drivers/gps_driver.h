
#include <TinyGPS++.h>
#include <HardwareSerial.h>

/**
 * baud => 9600 por defecto muchos modems gps
 * rxPin => pin rx
 * txPin => pin tx
 * trackInterval => frecuencia o intervalo de localización para envíar a un servidor por udp/mqtt/etc
 * callback => función de retorno con el payload a enviar por udp
 */
void setupGPSDriver(unsigned long baud, int8_t rxPin, int8_t txPin, const int trackInterval, 
    void (*callback)(String payload) );
    
// solo bloquea si es q hay data disponible dentro del modem gps, y ese bloqueo crecerá segun el buffer...
void readLocationAvailable();