#ifndef APDU_HANDLER_H
#define APDU_HANDLER_H

#include <stdint.h>
#include "uFCoder_ESP32.h"

class APDUHandler {
private:
    bool reader_opened;
    
    void logAPDUCommand(const uint8_t* command, uint32_t command_len, const uint8_t* response, uint32_t response_len);
    void logAPDUHexCommand(const char* hex_command, const char* hex_response);
    void interpretStatusWords(uint16_t sw);

public:
    APDUHandler();
    ~APDUHandler();
    
    bool initReader();
    void closeReader();
    
    bool sendAPDUCommand(const uint8_t* command, uint32_t command_len, uint8_t* response, uint32_t* response_len);
    bool sendAPDUHexString(const char* hex_command, char** hex_response);
    bool sendSelectCommand(const uint8_t* aid, uint8_t aid_len);
};

#endif // APDU_HANDLER_H