#include <Arduino.h>
#include "apdu_handler.h"

APDUHandler* apduHandler = nullptr;

void setup_apdu_example() {
    Serial.begin(115200);
    delay(2000);
    
    Serial.println("=== uFCoder APDU Example ===");
    
    apduHandler = new APDUHandler();
    
    if (!apduHandler->initReader()) {
        Serial.println("Failed to initialize RFID reader");
        return;
    }
    
    Serial.println("RFID reader initialized successfully");
    Serial.println("Ready to capture APDU commands...");
}

void loop_apdu_example() {
    if (apduHandler == nullptr) {
        return;
    }
    
    // Ejemplo de comandos APDU que puedes enviar
    
    // 1. Comando SELECT para seleccionar una aplicación
    uint8_t test_aid[] = {0xA0, 0x00, 0x00, 0x00, 0x03, 0x10, 0x10}; // Ejemplo de AID
    Serial.println("Sending SELECT command...");
    apduHandler->sendSelectCommand(test_aid, sizeof(test_aid));
    
    delay(3000);
    
    // 2. Comando GET DATA genérico
    uint8_t get_data_cmd[] = {0x00, 0xCA, 0x9F, 0x13, 0x00}; // GET DATA para obtener información
    uint8_t response[256];
    uint32_t response_len = sizeof(response);
    
    Serial.println("Sending GET DATA command...");
    apduHandler->sendAPDUCommand(get_data_cmd, sizeof(get_data_cmd), response, &response_len);
    
    delay(3000);
    
    // 3. Comando usando string hexadecimal
    const char* hex_cmd = "00A404000A000000000310100000000000";
    char* hex_response = nullptr;
    
    Serial.println("Sending HEX command...");
    apduHandler->sendAPDUHexString(hex_cmd, &hex_response);
    
    if (hex_response != nullptr) {
        // Liberar memoria asignada por la librería
        // La librería uFCoder maneja la memoria internamente
    }
    
    delay(5000);
}