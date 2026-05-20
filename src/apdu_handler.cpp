#include "apdu_handler.h"
#include <Arduino.h>

APDUHandler::APDUHandler() : reader_opened(false) {
}

APDUHandler::~APDUHandler() {
    if (reader_opened) {
        closeReader();
    }
}

bool APDUHandler::initReader() {
    UFR_STATUS status = ReaderOpen();
    
    if (status == UFR_OK) {
        reader_opened = true;
        Serial.println("uFCoder reader initialized successfully");
        return true;
    } else {
        Serial.printf("uFCoder reader not available. Status: 0x%04X\n", status);
        if (status == UFR_READER_NOT_CONNECTED) {
            Serial.println("Running in simulation mode - APDU commands will be logged but not sent to hardware");
        }
        return false;
    }
}

void APDUHandler::closeReader() {
    if (reader_opened) {
        ReaderClose();
        reader_opened = false;
        Serial.println("uFCoder reader closed");
    }
}

bool APDUHandler::sendAPDUCommand(const uint8_t* command, uint32_t command_len, uint8_t* response, uint32_t* response_len) {
    // Siempre log el comando, aunque no tengamos hardware
    Serial.println("=== SENDING APDU COMMAND ===");
    Serial.print("C-APDU: ");
    for (uint32_t i = 0; i < command_len; i++) {
        Serial.printf("%02X ", command[i]);
    }
    Serial.println();

    UFR_STATUS status = APDUPlainTransceive(command, command_len, response, response_len);
    
    if (status == UFR_OK) {
        Serial.println("APDU command sent successfully");
        logAPDUCommand(command, command_len, response, *response_len);
        return true;
    } else {
        Serial.printf("APDU simulation - Status: 0x%04X", status);
        if (status == UFR_NO_CARD_DETECTED || status == UFR_READER_NOT_CONNECTED) {
            Serial.println(" (Expected - no physical hardware)");
            // Log la respuesta simulada si está disponible
            if (response && response_len && *response_len > 0) {
                logAPDUCommand(command, command_len, response, *response_len);
            }
        } else {
            Serial.println();
        }
        return false;
    }
}

bool APDUHandler::sendAPDUHexString(const char* hex_command, char** hex_response) {
    if (!reader_opened) {
        Serial.println("Reader not initialized");
        return false;
    }

    UFR_STATUS status = APDUHexStrTransceive(hex_command, hex_response);
    
    if (status == UFR_OK) {
        Serial.println("APDU hex command sent successfully");
        logAPDUHexCommand(hex_command, *hex_response);
        return true;
    } else {
        Serial.printf("APDU hex command failed. Status: 0x%04X\n", status);
        return false;
    }
}

void APDUHandler::logAPDUCommand(const uint8_t* command, uint32_t command_len, const uint8_t* response, uint32_t response_len) {
    Serial.println("=== APDU COMMAND CAPTURE ===");
    
    Serial.print("C-APDU (Command): ");
    for (uint32_t i = 0; i < command_len; i++) {
        Serial.printf("%02X ", command[i]);
    }
    Serial.println();
    
    Serial.print("R-APDU (Response): ");
    for (uint32_t i = 0; i < response_len; i++) {
        Serial.printf("%02X ", response[i]);
    }
    Serial.println();
    
    if (response_len >= 2) {
        uint16_t sw = (response[response_len-2] << 8) | response[response_len-1];
        Serial.printf("Status Words (SW1SW2): %04X\n", sw);
        interpretStatusWords(sw);
    }
    
    Serial.println("=========================");
}

void APDUHandler::logAPDUHexCommand(const char* hex_command, const char* hex_response) {
    Serial.println("=== APDU HEX COMMAND CAPTURE ===");
    Serial.printf("C-APDU (Hex): %s\n", hex_command);
    Serial.printf("R-APDU (Hex): %s\n", hex_response);
    
    if (strlen(hex_response) >= 4) {
        char sw_str[5] = {0};
        strncpy(sw_str, hex_response + strlen(hex_response) - 4, 4);
        uint16_t sw = strtol(sw_str, NULL, 16);
        Serial.printf("Status Words (SW1SW2): %04X\n", sw);
        interpretStatusWords(sw);
    }
    
    Serial.println("==============================");
}

void APDUHandler::interpretStatusWords(uint16_t sw) {
    switch (sw) {
        case 0x9000:
            Serial.println("Status: SUCCESS");
            break;
        case 0x6300:
            Serial.println("Status: OPERATION FAILED");
            break;
        case 0x6700:
            Serial.println("Status: WRONG LENGTH");
            break;
        case 0x6982:
            Serial.println("Status: SECURITY STATUS NOT SATISFIED");
            break;
        case 0x6A80:
            Serial.println("Status: WRONG DATA");
            break;
        case 0x6A82:
            Serial.println("Status: FILE NOT FOUND");
            break;
        case 0x6D00:
            Serial.println("Status: INSTRUCTION NOT SUPPORTED");
            break;
        default:
            Serial.printf("Status: UNKNOWN (0x%04X)\n", sw);
            break;
    }
}

bool APDUHandler::sendSelectCommand(const uint8_t* aid, uint8_t aid_len) {
    uint8_t select_cmd[261];
    uint8_t response[261];
    uint32_t response_len = sizeof(response);
    
    select_cmd[0] = 0x00; // CLA
    select_cmd[1] = 0xA4; // INS (SELECT)
    select_cmd[2] = 0x04; // P1 (Select by name)
    select_cmd[3] = 0x00; // P2
    select_cmd[4] = aid_len; // Lc
    
    memcpy(&select_cmd[5], aid, aid_len);
    
    uint32_t total_len = 5 + aid_len;
    
    Serial.print("Sending SELECT command for AID: ");
    for (int i = 0; i < aid_len; i++) {
        Serial.printf("%02X ", aid[i]);
    }
    Serial.println();
    
    return sendAPDUCommand(select_cmd, total_len, response, &response_len);
}