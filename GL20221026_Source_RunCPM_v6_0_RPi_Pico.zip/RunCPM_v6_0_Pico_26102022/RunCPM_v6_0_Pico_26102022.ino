/*
  SD card connection

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
   // Arduino-pico core
   ** MISO - Pin 21 - GPIO 16
   ** MOSI - Pin 25 - GPIO 19
   ** CS   - Pin 22 - GPIO 17
   ** SCK  - Pin 24 - GPIO 18
*/

// only AVR and ARM CPU
// #include <MemoryFree.h>

#include "globals.h"

// =========================================================================================
// Guido Lehwalder's Code-Revision-Number
// =========================================================================================
#define GL_REV "GL20221026.0"

#include <SPI.h>

#include <SdFat.h>        // One SD library to rule them all - Greinman SdFat from Library Manager
// include <ESP8266SdFat.h>    // SdFat-version from RP2040 arduino-core

// =========================================================================================
// Board definitions go into the "hardware" folder, if you use a board different than the
// Arduino DUE, choose/change a file from there and reference that file here
// =========================================================================================

// Raspberry Pi Pico - normal (LED = GPIO25)
#include "hardware/pico/tiny_sd_rc2040_spi.h"

// Raspberry Pi Pico W(iFi)   (LED = GPIO32)
// #include "hardware/pico/pico_w_sd_spi.h"

// =========================================================================================
// Delays for LED blinking
// =========================================================================================
#define sDELAY 100
#define DELAY 1200

#include "abstraction_arduino.h"

// =========================================================================================
// Serial port speed
// =========================================================================================
#define SERIALSPD 115200

// =========================================================================================
// PUN: device configuration
// =========================================================================================
#ifdef USE_PUN
File32 pun_dev;
int pun_open = FALSE;
#endif

// =========================================================================================
// LST: device configuration
// =========================================================================================
#ifdef USE_LST
File32 lst_dev;
int lst_open = FALSE;
#endif

#include "ram.h"
#include "console.h"
#include "cpu.h"
#include "disk.h"
#include "host.h"
#include "cpm.h"
#ifdef CCP_INTERNAL
#include "ccp.h"
#endif

void setup(void) {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

// =========================================================================================
// Serial Port Definition
// =========================================================================================
//   Serial =USB / Serial1 =UART0/COM1 / Serial2 =UART1/COM2

//   Serial1.setRX(1); // Pin 2
//   Serial1.setTX(0); // Pin 1

//   Serial2.setRX(5); // Pin 7
//   Serial2.setTX(4); // Pin 6

// or

//   Serial1.setRX(17); // Pin 22
//   Serial1.setTX(16); // Pin 21

//   Serial2.setRX(21); // Pin 27
//   Serial2.setTX(20); // Pin 26
// =========================================================================================


  Serial.begin(SERIALSPD, SERIAL_8N1);
  while (!Serial) {	// Wait until serial is connected
    digitalWrite(LED, HIGH^LEDinv);
    delay(sDELAY);
    digitalWrite(LED, LOW^LEDinv);
    delay(DELAY);
  }

#ifdef DEBUGLOG
  _sys_deletefile((uint8 *)LogName);
#endif

  _clrscr();
  _puts("CP/M Emulator \e[1mv" VERSION "\e[0m   by   \e[1mMarcelo  Dantas\e[0m\r\n");
  _puts("----------------------------------------------\r\n");
  _puts("     running    on   Raspberry Pi [\e[1mTiny2040\e[0m]\r\n");
//   _puts("     running    on   Raspberry Pi [\e[1mPico-W\e[0m]\r\n");
  _puts("     compiled with   RP2040       [\e[1mv2.6.2\e[0m] \r\n");
  _puts("               and   ESP8266SdFat [\e[1mv2.1.1\e[0m] \r\n");
//   _puts("               and   SDFat        [\e[1mv2.2.0\e[0m] \r\n");
  _puts("----------------------------------------------\r\n");
  _puts("Revision             [\e[1m");
  _puts(GL_REV);
  _puts("\e[0m]\r\n");

	_puts("BIOS              at [\e[1m0x");
	_puthex16(BIOSjmppage);
//	_puts(" - ");
	_puts("\e[0m]\r\n");

	_puts("BDOS              at [\e[1m0x");
	_puthex16(BDOSjmppage);
	_puts("\e[0m]\r\n");

	_puts("CCP " CCPname " at [\e[1m0x");
	_puthex16(CCPaddr);
	_puts("\e[0m]\r\n");

  #if BANKS > 1
	_puts("Banked Memory        [\e[1m");
	_puthex8(BANKS);
    _puts("\e[0m]banks\r\n");
  #else
	_puts("Banked Memory        [\e[1m");
	_puthex8(BANKS);
	_puts("\e[0m]bank\r\n");
  #endif

   // Serial.printf("Free Memory          [\e[1m%d bytes\e[0m]\r\n", freeMemory());

  _puts("CPU-Clock            [\e[1m250Mhz\e[0m]\r\n");


// =========================================================================================
// SPIINIT !! ONLY !! for ESP32-Boards = #define board_esp32
// =========================================================================================
#if defined board_esp32
  _puts("Initializing  SPI    [ ");
  SPI.begin(SPIINIT);
  _puts("\e[1mDone\e[0m ]\r\n");
#endif

// =========================================================================================
// Redefine SPI-Pins - if needed : (SPI.) = SPI0 / (SPI1.) = SPI1
// =========================================================================================
  SPI.setRX(4);   // MISO
  SPI.setCS(5);   // Card Select
  SPI.setSCK(6);  // Clock
  SPI.setTX(3);   // MOSI

// =========================================================================================
// Setup SD card writing settings
// Info at: https://github.com/greiman/SdFat/issues/285#issuecomment-823562829
// =========================================================================================

// older SDINIT config
#define SDINIT SS, SD_SCK_MHZ(SDMHZ)

// #define SPI_SPEED SPI_FULL_SPEED           // full speed is 50Mhz - to fast for the most SDCard
// #define SPI_FULL_SPEED SD_SCK_MHZ(50)      // full speed is 50Mhz - to fast for the most SDCard

// older SD_CONFIG sample
// #define SD_CONFIG SdSpiConfig(5, DEDICATED_SPI, SPI_FULL_SPEED, &SPI)

// =========================================================================================
// NEW SD_CONFIG formerly SDINIT
// =========================================================================================
#define SDFAT_FILE_TYPE 1           // Uncomment for Due, Teensy or RPi Pico
#define ENABLE_DEDICATED_SPI 1      // Dedicated SPI 1=ON 0=OFF
#define SDMHZ_TXT "19"              // for outputing SDMHZ-Text
// normal is 12Mhz because of https://www.pschatzmann.ch/home/2021/03/14/rasperry-pico-with-the-sdfat-library/
#define SDMHZ 19                    // setting 19 Mhz for SPI-Bus
#define SS 5
// select required SPI-Bus : (&SPI) = SPI0 / (&SPI1) = SPI1
#define SD_CONFIG SdSpiConfig(SS, DEDICATED_SPI, SD_SCK_MHZ(SDMHZ), &SPI)
// #define SD_CONFIG SdSpiConfig(SS, DEDICATED_SPI, SPI_FULL_SPEED, &SPI)
// =========================================================================================


// =========================================================================================
// SPI SD Calibrate Test
// =========================================================================================
// int CALMHZ;
// CALMHZ=50;
// #define CALMHZ 50
// #define SD_CALIBRATE SdSpiConfig(SS, DEDICATED_SPI, SD_SCK_MHZ(CALMHZ), &SPI)
// _puts("While begin...\r\n");
// while (!SD.begin(SD_CALIBRATE) && CALMHZ > 1) {
//
//   Serial.printf("SD-Access failed at MHZ:  %d \r\n", CALMHZ);
//   CALMHZ = CALMHZ - 1;
//   #define SD_CALIBRATE SdSpiConfig(SS, DEDICATED_SPI, SD_SCK_MHZ(CALMHZ), &SPI)
//
//                                  }
// Serial.printf("DEBUG: SD-Access failed at MHZ:  %d \r\n", CALMHZ);
// _puts("While end...\r\n");

  _puts("Init MicroSD-Card    [ \e[1m");
//  old SDINIT
//  if (SD.begin(SDINIT)) {

// NEW SDCONFIG = formerly SDINIT
if (SD.begin(SD_CONFIG)) {
                        _puts(SDMHZ_TXT);
                        _puts("Mhz\e[0m]\r\n");
  _puts("----------------------------------------------");


    if (VersionCCP >= 0x10 || SD.exists(CCPname)) {
      while (true) {
        _puts(CCPHEAD);
        _PatchCPM();
	Status = 0;
#ifndef CCP_INTERNAL
        if (!_RamLoad((char *)CCPname, CCPaddr)) {
          _puts("Unable to load the CCP.\r\nCPU halted.\r\n");
          break;
        }
        Z80reset();
        SET_LOW_REGISTER(BC, _RamRead(DSKByte));
        PC = CCPaddr;
        Z80run();
#else
        _ccp();
#endif
        if (Status == 1)
          break;
#ifdef USE_PUN
        if (pun_dev)
          _sys_fflush(pun_dev);
#endif
#ifdef USE_LST
        if (lst_dev)
          _sys_fflush(lst_dev);
#endif
      }
    } else {
      _puts("Unable to load CP/M CCP.\r\nCPU halted.\r\n");
    }
  } else {
    _puts("Unable to initialize SD card.\r\nCPU halted.\r\n");
  }
}

void loop(void) {
  digitalWrite(LED, HIGH^LEDinv);
  delay(DELAY);
  digitalWrite(LED, LOW^LEDinv);
  delay(DELAY);
  digitalWrite(LED, HIGH^LEDinv);
  delay(DELAY);
  digitalWrite(LED, LOW^LEDinv);
  delay(DELAY * 4);
}
