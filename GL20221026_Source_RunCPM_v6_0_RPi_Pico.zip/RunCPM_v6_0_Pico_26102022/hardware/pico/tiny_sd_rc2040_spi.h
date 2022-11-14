// =========================================================================================
// Define SdFat as alias for SD
// =========================================================================================
SdFat SD;

// =========================================================================================
// Define Board-Data
// GPIO 25 oboard green   LED
// GPIO  6 RC2040 PCB red LED
// =========================================================================================
#define LED 20     // GPIO  6 RC2040 PCB red LED
#define RLED 18     // GPIO  6 RC2040 PCB red LED
#define GLED 19    // GPIO 25 oboard green   LED
#define LEDinv 1
#define board_pico
#define board_analog_io
#define board_digital_io
#define BOARD "Pimoroni Tiny2040"

// =========================================================================================
// SPIINIT !!ONLY!! for ESP32-boards
// #define SPIINIT Clock, MISO, MOSI, Card-Select
// =========================================================================================
#define SPIINIT 6,4,3,SS
#define SPIINIT_TXT "6,4,3,5"

// =========================================================================================
// Pin Documentation
// =========================================================================================
// Pimoroni Tiny2040
// MISO - Pin 12 - GPIO 4
// MOSI - Pin 13 - GPIO 3
// CS   - Pin 11 - GPIO 5
// SCK  - Pin 10 - GPIO 6
