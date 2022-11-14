// =========================================================================================
// Define SdFat as alias for SD
// =========================================================================================
SdFat SD;

// =========================================================================================
// Define Board-Data
// GPIO 25 oboard green   LED
// GPIO  6 RC2040 PCB red LED
// =========================================================================================
#define LED 25     // GPIO  6 RC2040 PCB red LED
#define RLED 6     // GPIO  6 RC2040 PCB red LED
#define GLED 25    // GPIO 25 oboard green   LED
#define LEDinv 0
#define board_pico
#define board_analog_io
#define board_digital_io
#define BOARD "RC2040 Pico"

// =========================================================================================
// SPIINIT !!ONLY!! for ESP32-boards
// #define SPIINIT Clock, MISO, MOSI, Card-Select
// =========================================================================================
#define SPIINIT 2,4,3,SS 
#define SPIINIT_TXT "2,4,3,5"

// =========================================================================================
// Pin Documentation
// =========================================================================================
// Normal RPi Pico 
// MISO - Pin 21 - GPIO 16
// MOSI - Pin 25 - GPIO 19
// CS   - Pin 22 - GPIO 17
// SCK  - Pin 24 - GPIO 18
 
// MicroSD Pin Definition for RC2040 board
// Pin 6 - GPIO 4 MISO
// Pin 7 - GPIO 5 Chip/Card-Select (CS / SS)
// Pin 4 - GPIO 2 Clock (SCK)
// Pin 5 - GPIO 3 MOSI
