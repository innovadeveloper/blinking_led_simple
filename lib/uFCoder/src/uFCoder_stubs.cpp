#ifdef ESP_PLATFORM

#include "uFCoder_ESP32.h"
#include <Arduino.h>

// Implementaciones stub para testing sin hardware real
extern "C" {

UFR_STATUS DL_API ReaderOpen(void) {
    Serial.println("[uFCoder Stub] ReaderOpen() called - No physical hardware detected");
    return UFR_READER_NOT_CONNECTED;
}

UFR_STATUS DL_API ReaderClose(void) {
    Serial.println("[uFCoder Stub] ReaderClose() called");
    return UFR_OK;
}

UFR_STATUS DL_API ReaderOpenEx(uint32_t reader_type, IN c_string port_name, uint32_t port_interface, IN void *arg) {
    Serial.printf("[uFCoder Stub] ReaderOpenEx(%u, %s, %u) called - No physical hardware detected\n", 
                  reader_type, port_name ? port_name : "NULL", port_interface);
    return UFR_READER_NOT_CONNECTED;
}

UFR_STATUS DL_API APDUPlainTransceive(IN const uint8_t *c_apdu, uint32_t c_apdu_len, OUT uint8_t *r_apdu, VAR uint32_t *r_apdu_len) {
    Serial.print("[uFCoder Stub] APDUPlainTransceive() - C-APDU: ");
    for (uint32_t i = 0; i < c_apdu_len; i++) {
        Serial.printf("%02X ", c_apdu[i]);
    }
    Serial.println();
    
    // Simular respuesta de error estándar (sin lector)
    if (r_apdu && r_apdu_len && *r_apdu_len >= 2) {
        r_apdu[0] = 0x6F; // SW1: Card error
        r_apdu[1] = 0x00; // SW2: No specific diagnosis
        *r_apdu_len = 2;
        Serial.println("[uFCoder Stub] R-APDU: 6F00 (No reader available)");
    }
    
    return UFR_NO_CARD_DETECTED;
}

UFR_STATUS DL_API APDUHexStrTransceive(IN const char *c_apdu, OUT char **r_apdu) {
    Serial.printf("[uFCoder Stub] APDUHexStrTransceive() - C-APDU: %s\n", c_apdu ? c_apdu : "NULL");
    
    if (r_apdu) {
        // Simular respuesta de error
        static char error_response[] = "6F00";
        *r_apdu = error_response;
        Serial.println("[uFCoder Stub] R-APDU: 6F00 (No reader available)");
    }
    
    return UFR_NO_CARD_DETECTED;
}

UFR_STATUS DL_API APDUTransceive(uint8_t cls, uint8_t ins, uint8_t p1, uint8_t p2, 
                                  IN const uint8_t *data_out, uint32_t Nc, 
                                  OUT uint8_t *data_in, VAR uint32_t *Nr, 
                                  uint8_t send_le, OUT uint8_t *apdu_status) {
    Serial.printf("[uFCoder Stub] APDUTransceive(%02X, %02X, %02X, %02X) called\n", cls, ins, p1, p2);
    
    if (apdu_status) {
        apdu_status[0] = 0x6F; // SW1
        apdu_status[1] = 0x00; // SW2
    }
    
    if (Nr) {
        *Nr = 0;
    }
    
    return UFR_NO_CARD_DETECTED;
}

UFR_STATUS DL_API uFR_APDU_Transceive(uint8_t cls, uint8_t ins, uint8_t p1, uint8_t p2, 
                                       IN uint8_t *data_out, uint8_t data_out_len, 
                                       OUT uint8_t *data_in, uint8_t max_data_in_len, 
                                       VAR uint8_t *response_len, uint8_t send_le, 
                                       OUT uint8_t *apdu_status) {
    Serial.printf("[uFCoder Stub] uFR_APDU_Transceive(%02X, %02X, %02X, %02X) called\n", cls, ins, p1, p2);
    
    if (apdu_status) {
        apdu_status[0] = 0x6F; // SW1
        apdu_status[1] = 0x00; // SW2
    }
    
    if (response_len) {
        *response_len = 0;
    }
    
    return UFR_NO_CARD_DETECTED;
}

} // extern "C"

#endif // ESP_PLATFORM