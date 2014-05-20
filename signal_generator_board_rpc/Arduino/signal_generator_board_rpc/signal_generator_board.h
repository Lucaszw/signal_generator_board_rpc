#ifndef _SIGNAL_GENERATOR_BOARD_H_
#define _SIGNAL_GENERATOR_BOARD_H_

#include "Arduino.h"
#include <stdint.h>

/* [`LTC6903`][1]: 1kHz – 68MHz Serial Port Programmable Oscillator.
 *
 * [1]: http://www.linear.com/product/LTC6903 */

// Arduino digital pin 11 (MOSI) to LTC6903 pin 2
// Arduino digital pin 13 (SCK) to LTC6903 pin 3
// Arduino digital pin 2 to LTC6903 pin 4

class SignalGeneratorClass {
public:
  struct version_t {
    uint16_t major;
    uint16_t minor;
    uint16_t micro;
  };

  struct config_settings_t {
    version_t version;
    uint8_t i2c_address;
    uint8_t pot[6];
    float hf_amplitude_correction;
    float max_voltage_rms;
  };

  // reserved return codes
  static const int8_t RETURN_OK                   = 0;
  static const int8_t RETURN_GENERAL_ERROR        = -1;
  static const int8_t RETURN_TIMEOUT              = -2;
  static const int8_t RETURN_NOT_CONNECTED        = -3;
  static const int8_t RETURN_BAD_INDEX            = -4;
  static const int8_t RETURN_BAD_PACKET_SIZE      = -5;
  static const int8_t RETURN_BAD_CRC              = -6;
  static const int8_t RETURN_BAD_VALUE            = -7;
  static const int8_t RETURN_MAX_PAYLOAD_EXCEEDED = -8;

  static const uint16_t MAX_PAYLOAD_LENGTH = 32;
  static const uint32_t BAUD_RATE          = 115200;
  static const int LTC6903_SS_PIN          = 2;
  static const int AD5206_SS_PIN           = 3;
  static const int FREQ_RANGE_PIN          = 4;
  static const float F_MIN                 = 100.0;
  static const float F_MAX                 = 10e3;
  static const float R_MAX                 = 100e3;
  static const float C1                    = 47e-9;
  static const float C2                    = 6.8e-9;
  static const float LOG_F_STEP;
  static const char R1_INDEX[] PROGMEM;
  static const char R2_INDEX[] PROGMEM;
  static const char R4_INDEX[] PROGMEM;
  static const char R5_INDEX[] PROGMEM;

  static const uint16_t EEPROM_CONFIG_SETTINGS = 0;

  void begin();
  // local accessors
  const char* name() { return prog_string(NAME_); }
  const char* hardware_version() { return prog_string(HARDWARE_VERSION_); }
  const char* url() { return prog_string(URL_); }
  const char* software_version() { return prog_string(SOFTWARE_VERSION_); }
  const char* protocol_name() { return prog_string(PROTOCOL_NAME_); }
  const char* protocol_version() { return prog_string(PROTOCOL_VERSION_); }
  const char* manufacturer() { return prog_string(MANUFACTURER_); }
  const char* prog_string(const char* str) { strcpy_P(buffer_, str); return buffer_; }

  void Listen();

  uint8_t pot(uint8_t index) const { return pot_[index]; }
  void set_pot(uint8_t address, uint8_t level, bool save_to_eeprom=false);
  float waveform_frequency() const { return waveform_frequency_; }
  void set_waveform_frequency(float freq);
  float waveform_voltage() const { return waveform_voltage_; }
  uint8_t set_waveform_voltage(float vrms);
  void set_i2c_address(uint8_t address);
  void set_hf_amplitude_correction(float correction);
  float vout_pk_pk();
  version_t ConfigVersion();
  char buffer_[MAX_PAYLOAD_LENGTH];
  void LoadConfig(bool use_defaults=false);
private:
  String VersionString(version_t version);
  void SaveConfig();

  static const char SOFTWARE_VERSION_[] PROGMEM;
  static const char NAME_[] PROGMEM;
  static const char HARDWARE_VERSION_[] PROGMEM;
  static const char MANUFACTURER_[] PROGMEM;
  static const char URL_[] PROGMEM;
  static const char PROTOCOL_NAME_[] PROGMEM;
  static const char PROTOCOL_VERSION_[] PROGMEM;
  config_settings_t config_settings_;
  float waveform_frequency_;
  float waveform_voltage_;
  uint8_t pot_[6];
};

extern SignalGeneratorClass SignalGeneratorBoard;

#endif // _SIGNAL_GENERATOR_BOARD_H_
