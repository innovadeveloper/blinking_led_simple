/*
 * ESP32 uFR Client - SAM Command Executor
 * 
 * Uses ESP32 Arduino port of uFCoder library to execute SAM commands
 * and capture/log the APDU communication
 * 
 * Hardware: ESP32 + uFR Reader + SAM card
 * 
 * This approach uses the actual uFCoder library on ESP32 to call
 * SAM_change_key_entry_aes_AV2_plain_host_key and capture results
 */

#include "uFCoder.h"  // ESP32 port of uFCoder library

// Configuration
#define DEBUG_SERIAL Serial
#define UFR_BAUDRATE 115200

// Test parameters for SAM commands
uint8_t test_key_a[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                          0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

uint8_t test_key_b[16] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                          0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};

uint8_t test_key_c[16] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
                          0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F};

void setup() {
  DEBUG_SERIAL.begin(115200);
  delay(2000);
  
  DEBUG_SERIAL.println("ESP32 uFR SAM Command Client");
  DEBUG_SERIAL.println("============================");
  DEBUG_SERIAL.println();
  
  // Initialize uFR reader connection
  DEBUG_SERIAL.println("1. Initializing uFR connection...");
  
  UFR_STATUS status = ReaderOpen();
  DEBUG_SERIAL.printf("   ReaderOpen Status: 0x%08X", status);
  
  if (status == UFR_OK) {
    DEBUG_SERIAL.println(" (SUCCESS)");
    
    // Get reader information
    DEBUG_SERIAL.println("\n2. Getting reader information...");
    
    uint32_t reader_type, reader_serial;
    status = GetReaderType(&reader_type);
    DEBUG_SERIAL.printf("   Reader Type: 0x%08X (Status: 0x%08X)\n", reader_type, status);
    
    status = GetReaderSerialNumber(&reader_serial);
    DEBUG_SERIAL.printf("   Reader Serial: 0x%08X (Status: 0x%08X)\n", reader_serial, status);
    
    // Test SAM functions
    testSAMFunctions();
    
  } else {
    DEBUG_SERIAL.printf(" (FAILED - 0x%08X)\n", status);
    DEBUG_SERIAL.println("   Please check:");
    DEBUG_SERIAL.println("   - uFR reader connected to ESP32");
    DEBUG_SERIAL.println("   - Correct wiring (RX/TX pins)");
    DEBUG_SERIAL.println("   - Power supply");
  }
}

void loop() {
  // Main test completed in setup()
  delay(10000);
  
  // Optionally repeat tests
  DEBUG_SERIAL.println("\n--- Repeating SAM tests ---");
  testSAMFunctions();
}

void testSAMFunctions() {
  DEBUG_SERIAL.println("\n3. Testing SAM functions...");
  DEBUG_SERIAL.println("   (These will generate APDU commands to the SAM)");
  
  // Test 1: SAM Get Version
  DEBUG_SERIAL.println("\n   a) SAM_get_version():");
  testSAMGetVersion();
  
  // Test 2: SAM Get Version Raw  
  DEBUG_SERIAL.println("\n   b) SAM_get_version_raw():");
  testSAMGetVersionRaw();
  
  // Test 3: THE MAIN TARGET - SAM Change Key Entry
  DEBUG_SERIAL.println("\n   c) *** TARGET: SAM_change_key_entry_aes_AV2_plain_host_key ***");
  testSAMChangeKeyEntry();
  
  // Test 4: SAM Authenticate
  DEBUG_SERIAL.println("\n   d) SAM_authenticate_host_AV2_plain():");
  testSAMAuthenticate();
}

void testSAMGetVersion() {
  SAM_HW_TYPE sam_type;
  uint8_t sam_uid[7];
  
  UFR_STATUS status = SAM_get_version(&sam_type, sam_uid);
  
  DEBUG_SERIAL.printf("      Status: 0x%08X\n", status);
  DEBUG_SERIAL.printf("      SAM Type: %d\n", sam_type);
  DEBUG_SERIAL.printf("      SAM UID: ");
  for (int i = 0; i < 7; i++) {
    DEBUG_SERIAL.printf("%02X ", sam_uid[i]);
  }
  DEBUG_SERIAL.println();
  
  if (status != UFR_OK) {
    DEBUG_SERIAL.printf("      (Expected if no SAM card present)\n");
  }
}

void testSAMGetVersionRaw() {
  uint8_t raw_data[32];
  uint8_t raw_length = sizeof(raw_data);
  
  UFR_STATUS status = SAM_get_version_raw(raw_data, &raw_length);
  
  DEBUG_SERIAL.printf("      Status: 0x%08X\n", status);
  DEBUG_SERIAL.printf("      Length: %d\n", raw_length);
  DEBUG_SERIAL.printf("      Raw Data: ");
  for (int i = 0; i < raw_length && i < 32; i++) {
    DEBUG_SERIAL.printf("%02X ", raw_data[i]);
  }
  DEBUG_SERIAL.println();
}

void testSAMChangeKeyEntry() {
  uint8_t apdu_sw[2];
  
  DEBUG_SERIAL.println("      Parameters:");
  DEBUG_SERIAL.printf("        key_entry_no: 0x01\n");
  DEBUG_SERIAL.printf("        aes_key_ver_a: %02X %02X %02X %02X...\n", 
                     test_key_a[0], test_key_a[1], test_key_a[2], test_key_a[3]);
  DEBUG_SERIAL.printf("        ver_a: 0x01\n");
  DEBUG_SERIAL.printf("        aes_key_ver_b: %02X %02X %02X %02X...\n", 
                     test_key_b[0], test_key_b[1], test_key_b[2], test_key_b[3]);
  DEBUG_SERIAL.printf("        ver_b: 0x02\n");
  DEBUG_SERIAL.printf("        aes_key_ver_c: %02X %02X %02X %02X...\n", 
                     test_key_c[0], test_key_c[1], test_key_c[2], test_key_c[3]);
  DEBUG_SERIAL.printf("        ver_c: 0x03\n");
  
  DEBUG_SERIAL.println("      >>> Calling SAM_change_key_entry_aes_AV2_plain_host_key... <<<");
  
  UFR_STATUS status = SAM_change_key_entry_aes_AV2_plain_host_key(
    0x01,                    // key_entry_no
    test_key_a, 0x01,        // aes_key_ver_a, ver_a
    test_key_b, 0x02,        // aes_key_ver_b, ver_b
    test_key_c, 0x03,        // aes_key_ver_c, ver_c
    0x00,                    // key_no_CEK
    0x00,                    // key_v_CEK
    0x00,                    // ref_no_KUC
    0x00,                    // sam_lock_unlock
    0x01,                    // sam_auth_host
    apdu_sw                  // apdu_sw output
  );
  
  DEBUG_SERIAL.printf("      Result Status: 0x%08X\n", status);
  DEBUG_SERIAL.printf("      APDU SW1 SW2: %02X %02X\n", apdu_sw[0], apdu_sw[1]);
  
  if (status == UFR_OK) {
    DEBUG_SERIAL.println("      *** SUCCESS: Command executed! ***");
    
    // Analyze APDU response
    if (apdu_sw[0] == 0x90 && apdu_sw[1] == 0x00) {
      DEBUG_SERIAL.println("      *** APDU Response: SUCCESS (90 00) ***");
    } else {
      DEBUG_SERIAL.printf("      *** APDU Response: ERROR (%02X %02X) ***\n", apdu_sw[0], apdu_sw[1]);
    }
  } else {
    DEBUG_SERIAL.printf("      (Failed - likely no SAM or authentication needed)\n");
    
    // Common error codes
    switch (status) {
      case 0xA1:
        DEBUG_SERIAL.println("      Error: FTDI communication error");
        break;
      case 0x54:
        DEBUG_SERIAL.println("      Error: Reader opening error");
        break;
      default:
        DEBUG_SERIAL.printf("      Error: Unknown (0x%08X)\n", status);
    }
  }
}

void testSAMAuthenticate() {
  uint8_t host_key[16] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11,
                         0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
  uint8_t auth_sw[2];
  
  UFR_STATUS status = SAM_authenticate_host_AV2_plain(host_key, 0x00, 0x01, auth_sw);
  
  DEBUG_SERIAL.printf("      Status: 0x%08X\n", status);
  DEBUG_SERIAL.printf("      Auth SW: %02X %02X\n", auth_sw[0], auth_sw[1]);
}

void printHexArray(const char* label, uint8_t* data, int length) {
  DEBUG_SERIAL.printf("%s: ", label);
  for (int i = 0; i < length; i++) {
    DEBUG_SERIAL.printf("%02X ", data[i]);
    if ((i + 1) % 16 == 0) DEBUG_SERIAL.println();
  }
  if (length % 16 != 0) DEBUG_SERIAL.println();
}