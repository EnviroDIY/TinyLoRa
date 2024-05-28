/**
 * @file       LoRa_AT_Battery.tpp
 * @author     Sara Damiano
 * @copyright  Stroud Water Research Center
 * @date       May 2024
 */

#ifndef SRC_TINYLORABATTERY_H_
#define SRC_TINYLORABATTERY_H_

#include "LoRa_AT_Common.h"

#define LORA_AT_HAS_BATTERY

template <class modemType>
class LoRa_AT_Battery {
  /* =========================================== */
  /* =========================================== */
  /*
   * Define the interface
   */
 public:
  /*
   * Battery functions
   */
  int16_t getBattVoltage() {
    return thisModem().getBattVoltageImpl();
  }
  int8_t getBattPercent() {
    return thisModem().getBattPercentImpl();
  }
  int8_t getBattChargeState() {
    return thisModem().getBattChargeStateImpl();
  }
  bool getBattStats(int8_t& chargeState, int8_t& percent, int16_t& milliVolts) {
    return thisModem().getBattStatsImpl(chargeState, percent, milliVolts);
  }

  /*
   * CRTP Helper
   */
 protected:
  inline const modemType& thisModem() const {
    return static_cast<const modemType&>(*this);
  }
  inline modemType& thisModem() {
    return static_cast<modemType&>(*this);
  }
  ~LoRa_AT_Battery() {}

  /* =========================================== */
  /* =========================================== */
  /*
   * Define the default function implementations
   */

  /*
   * Battery functions
   */
 protected:
  // Use: float vBatt = modem.getBattVoltage() / 1000.0;
  int16_t getBattVoltageImpl() LORA_AT_ATTR_NOT_IMPLEMENTED;

  int8_t getBattPercentImpl() LORA_AT_ATTR_NOT_IMPLEMENTED;

  int8_t getBattChargeStateImpl() LORA_AT_ATTR_NOT_IMPLEMENTED;

  bool getBattStatsImpl(int8_t& chargeState, int8_t& percent,
                        int16_t& milliVolts) LORA_AT_ATTR_NOT_IMPLEMENTED;
};

#endif  // SRC_TINYLORABATTERY_H_