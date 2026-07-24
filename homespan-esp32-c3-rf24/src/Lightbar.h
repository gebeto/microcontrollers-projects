#pragma once

#include <Arduino.h>
#include <RF24.h>

// C++ port of the Python `Lightbar` (xiaomi_lightbar/radio.py) and the packet
// builder (xiaomi_lightbar/baseband.py). Controls a Xiaomi Mi Computer Monitor
// Light Bar (model MJGJD01YL, non-BLE / Telink TLSR8368) by spoofing its 2.4 GHz
// remote with an nRF24L01(+) module.
//
// The bar has a single toggle command (0x0100); there is no separate on/off,
// calling on_off() toggles the current state just like pressing the knob.
class Lightbar {
public:
  // remoteId is the 3-byte id of your remote (e.g. 0xABCDEF). Capture it with
  // scripts/scan_lightbar_remote.py, or use an arbitrary id and re-pair the bar.
  Lightbar(uint8_t cePin, uint8_t csnPin, uint32_t remoteId);

  // Initializes SPI + radio and applies the radio configuration. Returns true
  // if the nRF24 chip is responding.
  bool begin();

  bool isAvailable();

  // Sends a raw 2-byte command code. counter < 0 uses (and increments) the
  // internal rolling 0-255 counter used by the bar to reject duplicate packets.
  void send(uint16_t code, int counter = -1);

  // Toggle the light bar on/off (command 0x0100).
  void on_off(int counter = -1);

  // Returns the 17-byte packet as an uppercase hex string. Useful for the
  // offline self-test against the gold vector (id 0xABCDEF, cmd 0x0100,
  // counter 0x72 -> 533914DD1C493412ABCDEFFF720100FAD4).
  String packetHex(uint16_t command, uint8_t counter);

  // Number of times each packet is written, and the spacing between writes (ms).
  // Mirrors the Python defaults (20 repetitions, 10 ms apart).
  uint8_t repetitions = 20;
  uint16_t delayMs = 10;

private:
  void buildPacket(uint8_t out[17], uint16_t command, uint8_t counter);
  uint16_t crc16(const uint8_t *data, size_t len);

  RF24 radio;
  uint32_t id;
  uint8_t counter = 0;
};
