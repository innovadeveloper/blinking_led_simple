# uFCoder APDU Integration

Este documento explica cómo usar la integración de la librería uFCoder para capturar comandos APDU en tu ESP32.

## Configuración

La librería uFCoder se encuentra en `lib/uFCoder/` y está configurada automáticamente en `platformio.ini`.

### Solución de Problemas de Include

Si encuentras errores de include como `TargetConditionals.h`, la librería ahora usa `uFCoder_ESP32.h` que es un wrapper compatible con ESP32.

## Uso Básico

### 1. Inicialización

```cpp
#include "apdu_handler.h"

APDUHandler* apduHandler = new APDUHandler();
if (apduHandler->initReader()) {
    Serial.println("Reader initialized");
} else {
    Serial.println("No hardware detected - using simulation mode");
}
```

**Nota**: Sin hardware físico, la inicialización fallará pero mostrará mensajes informativos.

### 2. Envío de Comandos APDU

#### Comando binario:
```cpp
uint8_t command[] = {0x00, 0xA4, 0x04, 0x00, 0x07, 0xA0, 0x00, 0x00, 0x00, 0x03, 0x10, 0x10};
uint8_t response[256];
uint32_t response_len = sizeof(response);

apduHandler->sendAPDUCommand(command, sizeof(command), response, &response_len);
```

#### Comando hexadecimal:
```cpp
const char* hex_cmd = "00A404000A000000000310100000000000";
char* hex_response = nullptr;

apduHandler->sendAPDUHexString(hex_cmd, &hex_response);
```

#### Comando SELECT específico:
```cpp
uint8_t aid[] = {0xA0, 0x00, 0x00, 0x00, 0x03, 0x10, 0x10};
apduHandler->sendSelectCommand(aid, sizeof(aid));
```

## Captura de Comandos APDU

Todos los comandos enviados se registran automáticamente en el Serial Monitor con el formato:

```
=== APDU COMMAND CAPTURE ===
C-APDU (Command): 00 A4 04 00 07 A0 00 00 00 03 10 10 
R-APDU (Response): 6F 1A 84 07 A0 00 00 00 03 10 10 90 00 
Status Words (SW1SW2): 9000
Status: SUCCESS
=========================
```

## Funciones Principales de la Librería uFCoder

### Funciones de Conexión:
- `ReaderOpen()`: Abrir conexión con el lector
- `ReaderClose()`: Cerrar conexión
- `ReaderOpenEx()`: Abrir con parámetros específicos

### Funciones APDU:
- `APDUPlainTransceive()`: Envío binario de comandos APDU
- `APDUHexStrTransceive()`: Envío en formato hexadecimal
- `APDUTransceive()`: Comando APDU "exploded" con parámetros separados
- `uFR_APDU_Transceive()`: Función de bajo nivel

### Códigos de Estado Comunes:
- `0x9000`: SUCCESS
- `0x6300`: OPERATION FAILED  
- `0x6700`: WRONG LENGTH
- `0x6982`: SECURITY STATUS NOT SATISFIED
- `0x6A80`: WRONG DATA
- `0x6A82`: FILE NOT FOUND
- `0x6D00`: INSTRUCTION NOT SUPPORTED

## Simulación sin Hardware

Aunque no tengas un lector físico uFR, puedes:

1. **Compilar y probar la estructura**: El código compilará sin errores
2. **Simular comandos**: Los comandos fallarán gracefully mostrando códigos de error
3. **Desarrollar lógica**: Preparar toda la lógica APDU para cuando tengas el hardware

## Notas Importantes

- La librería está configurada con flags específicos para ESP32
- Los comandos APDU siguen el estándar ISO 7816-4
- Todos los comandos se registran para debugging
- La librería maneja automáticamente la memoria para respuestas hexadecimales