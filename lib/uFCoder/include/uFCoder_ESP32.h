#ifndef uFCoder_ESP32_H_
#define uFCoder_ESP32_H_

// ESP32-specific wrapper for uFCoder library
#ifdef ESP_PLATFORM

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define IN  // array that you pass to function
#define OUT // array that you receive from function
#define VAR // first element of array that you receive from function (single variable)

typedef const char *c_string;

// ESP32 specific definitions
#define DL_API
typedef struct S_UFR_HANDLE *UFR_HANDLE;

// Status codes (principales)
typedef enum E_UFR_STATUS {
    UFR_OK = 0x00,
    UFR_COMMUNICATION_ERROR = 0x01,
    UFR_CHKSUM_ERROR = 0x02,
    UFR_READING_ERROR = 0x03,
    UFR_WRITING_ERROR = 0x04,
    UFR_BUFFER_OVERFLOW = 0x05,
    UFR_MAX_ADDRESS_EXCEEDED = 0x06,
    UFR_MAX_KEY_INDEX_EXCEEDED = 0x07,
    UFR_NO_CARD = 0x08,
    UFR_COMMAND_NOT_SUPPORTED = 0x09,
    UFR_FORBIDEN_DIRECT_WRITE_IN_SECTOR_TRAILER = 0x0A,
    UFR_ADDRESSED_BLOCK_IS_NOT_SECTOR_TRAILER = 0x0B,
    UFR_WRONG_KEY_TYPE = 0x0C,
    UFR_KEY_AUTH_ERROR = 0x0D,
    UFR_SECTOR_NOT_AUTHENTICATED = 0x0E,
    UFR_SECTOR_ALREADY_AUTHENTICATED = 0x0F,
    UFR_NO_MEMORY_TO_ALLOCATE = 0x10,
    UFR_CAN_NOT_SAVE_FILE = 0x11,
    UFR_CAN_NOT_LOAD_FILE = 0x12,
    UFR_WRONG_FILE = 0x13,
    UFR_COMMUNICATION_BREAK = 0x50,
    UFR_NO_CARD_DETECTED = 0x51,
    UFR_READER_NOT_SUPPORTED = 0x65,
    UFR_READER_OPENING_ERROR = 0x66,
    UFR_READER_PORT_NOT_OPENED = 0x67,
    UFR_PORT_NOT_OPENED = 0x68,
    UFR_WRONG_READER_TYPE = 0x69,
    UFR_READER_NOT_CONNECTED = 0x6A,
    UFR_DEVICE_UNKNOWN = 0x6B,
    UFR_DEVICE_WRONG_TYPE = 0x6C,
    UFR_CAN_NOT_CLOSE_DEVICE = 0x6D,
    UFR_APDU_TRANSCEIVE_ERROR = 0xAE,
    UFR_APDU_SW_TAG = 0x000A0000,
    UFR_APDU_SW_OPERATION_IS_FAILED = 0x000A6300,
    UFR_APDU_SW_WRONG_LENGTH = 0x000A6700,
    UFR_APDU_SW_SECURITY_STATUS_NOT_SATISFIED = 0x000A6982,
    UFR_APDU_SW_WRONG_DATA = 0x000A6A80,
    UFR_APDU_SW_FILE_NOT_FOUND = 0x000A6A82,
    UFR_APDU_SW_INS_NOT_SUPPORTED = 0x000A6D00
} UFR_STATUS;

// Function declarations principales para APDU
extern "C" {
    UFR_STATUS DL_API ReaderOpen(void);
    UFR_STATUS DL_API ReaderClose(void);
    UFR_STATUS DL_API ReaderOpenEx(uint32_t reader_type, IN c_string port_name, uint32_t port_interface, IN void *arg);
    
    UFR_STATUS DL_API APDUPlainTransceive(IN const uint8_t *c_apdu, uint32_t c_apdu_len, OUT uint8_t *r_apdu, VAR uint32_t *r_apdu_len);
    UFR_STATUS DL_API APDUHexStrTransceive(IN const char *c_apdu, OUT char **r_apdu);
    UFR_STATUS DL_API APDUTransceive(uint8_t cls, uint8_t ins, uint8_t p1, uint8_t p2, IN const uint8_t *data_out, uint32_t Nc, OUT uint8_t *data_in, VAR uint32_t *Nr, uint8_t send_le, OUT uint8_t *apdu_status);
    UFR_STATUS DL_API uFR_APDU_Transceive(uint8_t cls, uint8_t ins, uint8_t p1, uint8_t p2, IN uint8_t *data_out, uint8_t data_out_len, OUT uint8_t *data_in, uint8_t max_data_in_len, VAR uint8_t *response_len, uint8_t send_le, OUT uint8_t *apdu_status);
}

#endif // ESP_PLATFORM

#endif // uFCoder_ESP32_H_