/**************************************************************
 *
 * This script tries to auto-detect the baud rate
 * and allows direct AT commands access
 *
 * TinyLoRa Getting Started guide:
 *   https://tiny.cc/TinyLoRa-readme
 *
 **************************************************************/

// Select your modem:
#define TINY_LORA_MDOT
// #define TINY_LORA_SIM900
// #define TINY_LORA_SIM808
// #define TINY_LORA_SIM868
// #define TINY_LORA_UBLOX
// #define TINY_LORA_M95
// #define TINY_LORA_BG96
// #define TINY_LORA_A6
// #define TINY_LORA_A7
// #define TINY_LORA_M590
// #define TINY_LORA_MC60
// #define TINY_LORA_MC60E
// #define TINY_LORA_ESP8266
// #define TINY_LORA_XBEE

// Set serial for debug console (to the Serial Monitor, speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
// Use Hardware Serial on Mega, Leonardo, Micro
#ifndef __AVR_ATmega328P__
#define SerialAT Serial1

// or Software Serial on Uno, Nano
#else
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(2, 3);  // RX, TX
#endif

#define TINY_LORA_DEBUG SerialMon

#include <TinyLoRa.h>

// Module baud rate
uint32_t rate = 0;  // Set to 0 for Auto-Detect

void setup() {
  // Set console baud rate
  SerialMon.begin(115200);
  delay(6000);
}

void loop() {
  if (!rate) { rate = TinyLoRaAutoBaud(SerialAT); }

  if (!rate) {
    SerialMon.println(
        F("***********************************************************"));
    SerialMon.println(F(" Module does not respond!"));
    SerialMon.println(F("   Check your Serial wiring"));
    SerialMon.println(
        F("   Check the module is correctly powered and turned on"));
    SerialMon.println(
        F("***********************************************************"));
    delay(30000L);
    return;
  }

  SerialAT.begin(rate);

  // Access AT commands from Serial Monitor
  SerialMon.println(
      F("***********************************************************"));
  SerialMon.println(F(" You can now send AT commands"));
  SerialMon.println(
      F(" Enter \"AT\" (without quotes), and you should see \"OK\""));
  SerialMon.println(
      F(" If it doesn't work, select \"Both NL & CR\" in Serial Monitor"));
  SerialMon.println(
      F("***********************************************************"));

  while (true) {
    if (SerialAT.available()) { SerialMon.write(SerialAT.read()); }
    if (SerialMon.available()) { SerialAT.write(SerialMon.read()); }
    delay(0);
  }
}
