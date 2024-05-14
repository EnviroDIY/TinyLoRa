/**
 * @file       TinyLoRaCommon.h
 * @author     Sara Damiano
 * @license    LGPL-3.0
 * @copyright  Stroud Water Research Center
 * @date       May 2024
 */

#ifndef SRC_TINYLORACOMMON_H_
#define SRC_TINYLORACOMMON_H_

// The current library version number
#define TINY_LORA_VERSION "0.11.7"

#if defined(SPARK) || defined(PARTICLE)
#include "Particle.h"
#elif defined(ARDUINO)
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#endif

#ifndef TINY_LORA_YIELD_MS
#define TINY_LORA_YIELD_MS 0
#endif

#ifndef TINY_LORA_YIELD
#define TINY_LORA_YIELD() \
  { delay(TINY_LORA_YIELD_MS); }
#endif

#if !defined(TINY_LORA_RX_BUFFER)
#define TINY_LORA_RX_BUFFER 256
#endif

#if !defined(TINY_LORA_DL_CHECK)
#define TINY_LORA_DL_CHECK 30000L
#endif

#define TINY_LORA_ATTR_NOT_AVAILABLE \
  __attribute__((error("Not available on this modem type")))
#define TINY_LORA_ATTR_NOT_IMPLEMENTED __attribute__((error("Not implemented")))

#if defined(__AVR__) && !defined(__AVR_ATmega4809__)
#define TINY_LORA_PROGMEM PROGMEM
typedef const __FlashStringHelper* GsmConstStr;
#define GFP(x) (reinterpret_cast<GsmConstStr>(x))
#define GF(x) F(x)
#else
#define TINY_LORA_PROGMEM
typedef const char* GsmConstStr;
#define GFP(x) x
#define GF(x) x
#endif

#ifdef TINY_LORA_DEBUG
namespace {
template <typename T>
static void DBG_PLAIN(T last) {
  TINY_LORA_DEBUG.println(last);
}

template <typename T, typename... Args>
static void DBG_PLAIN(T head, Args... tail) {
  TINY_LORA_DEBUG.print(head);
  TINY_LORA_DEBUG.print(' ');
  DBG_PLAIN(tail...);
}

template <typename... Args>
static void DBG(Args... args) {
  TINY_LORA_DEBUG.print(GF("["));
  TINY_LORA_DEBUG.print(millis());
  TINY_LORA_DEBUG.print(GF("] "));
  DBG_PLAIN(args...);
}
}  // namespace
#else
#define DBG_PLAIN(...)
#define DBG(...)
#endif

template <class T>
const T& TinyLoRaMin(const T& a, const T& b) {
  return (b < a) ? b : a;
}

template <class T>
const T& TinyLoRaMax(const T& a, const T& b) {
  return (b < a) ? a : b;
}

template <class T>
uint32_t TinyLoRaAutoBaud(T& SerialAT, uint32_t minimum = 9600,
                          uint32_t maximum = 115200) {
  static uint32_t rates[] = {115200, 57600,  38400, 19200, 9600,  74400, 74880,
                             230400, 460800, 2400,  4800,  14400, 28800};

  for (uint8_t i = 0; i < sizeof(rates) / sizeof(rates[0]); i++) {
    uint32_t rate = rates[i];
    if (rate < minimum || rate > maximum) continue;

    DBG("Trying baud rate", rate, "...");
    SerialAT.begin(rate);
    delay(10);
    for (int j = 0; j < 10; j++) {
      SerialAT.print("AT\r\n");
      String input = SerialAT.readString();
      if (input.indexOf("OK") >= 0) {
        DBG("Modem responded at rate", rate);
        return rate;
      }
    }
  }
  SerialAT.begin(minimum);
  return 0;
}

#if defined TINY_LORA_DEBUG
#define LORA_STATIC_VARIABLES(specificModem)                              \
  template <>                                                             \
  const char* TinyLoRaModem<specificModem>::modem_nl = AT_NL;             \
  template <>                                                             \
  const char* TinyLoRaModem<specificModem>::modem_ok = AT_OK AT_NL;       \
  template <>                                                             \
  const char* TinyLoRaModem<specificModem>::modem_error = AT_ERROR AT_NL; \
                                                                          \
  template <>                                                             \
  const char* TinyLoRaModem<specificModem>::modem_verbose = AT_VERBOSE;
#else
#define AT_STATIC_VARIABLES(specificModem)                          \
  template <>                                                       \
  const char* TinyLoRaModem<specificModem>::modem_nl = AT_NL;       \
  template <>                                                       \
  const char* TinyLoRaModem<specificModem>::modem_ok = AT_OK AT_NL; \
  template <>                                                       \
  const char* TinyLoRaModem<specificModem>::modem_error = AT_ERROR AT_NL
#endif

#endif  // SRC_TINYLORACOMMON_H_
