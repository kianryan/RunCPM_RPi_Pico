// =========================================================================================
// Define SdFat as alias for SD
// =========================================================================================
SdFat SD;

// =========================================================================================
// Define Board-Data
// GP25 green onboard LED
// =========================================================================================
#define LED 25  // GPIO25
#define LEDinv 0
#define board_pico
#define board_analog_io
#define board_digital_io
#define BOARD "Maker Pi Pico"

// =========================================================================================
// SPIINIT !!ONLY!! for ESP32-boards
// #define SPIINIT Clock, MISO, MOSI, Card-Select
// =========================================================================================
#define SPIINIT 10,11,12,SS 
#define SPIINIT_TXT "10,11,12,15"

// =========================================================================================
// Pin Documentation
// =========================================================================================
// Normal RPi Pico 
// MISO - Pin 21 - GPIO 16
// MOSI - Pin 25 - GPIO 19
// CS   - Pin 22 - GPIO 17
// SCK  - Pin 24 - GPIO 18

// MicroSD Pin Definition for Maker Pico
// Pin 14 - GPIO 10 - SCK/Clock
// Pin 15 - GPIO 11 - MISO
// Pin 16 - GPIO 12 - MOSI
// Pin 20 - GPIO 15 - CS/SS