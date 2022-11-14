// =========================================================================================
// Define SdFat as alias for SD
// =========================================================================================
SdFat SD;

// =========================================================================================
// Define Board-Data
// GP25 green onboard LED
// =========================================================================================
#define LED 32  // GPIO32 = onboard LED for Pico-W
#define LEDinv 0
#define board_pico
#define board_analog_io
#define board_digital_io
#define BOARD "Raspberry Pi Pico W"

// =========================================================================================
// SPIINIT !!ONLY!! for ESP32-boards
// #define SPIINIT Clock, MISO, MOSI, Card-Select
// =========================================================================================
#define SPIINIT 18,16,19,SS 
#define SPIINIT_TXT "18,16,19,17"

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
