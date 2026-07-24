#include "Lightbar.h"

#include "config.h"
#include <SPI.h>

// Baseband constants, common to all devices (xiaomi_lightbar/baseband.py).
static const uint8_t PREAMBLE[8] = {0x53, 0x39, 0x14, 0xDD,
                                    0x1C, 0x49, 0x34, 0x12};
static const uint8_t SEPARATOR = 0xFF;

Lightbar::Lightbar(uint8_t cePin, uint8_t csnPin, uint32_t remoteId)
    : radio(cePin, csnPin), id(remoteId) {}

bool Lightbar::begin() {
  SPI.begin(NRF_SCK_PIN, NRF_MISO_PIN, NRF_MOSI_PIN);

  if (!radio.begin(&SPI)) {
    return false;
  }

  radio.setChannel(6); // 2406 MHz (try 15/43/68 if the bar does not respond)
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_2MBPS);
  radio.setRetries(0, 0); // no auto-retransmit; repetition is done manually
  radio.disableDynamicPayloads();
  radio.setPayloadSize(17);

  // 5-byte "address" = 0x5555555555, stands in for the Telink sync sequence.
  const uint8_t txAddress[5] = {0x55, 0x55, 0x55, 0x55, 0x55};
  radio.openWritingPipe(txAddress);

  radio.stopListening(); // TX mode

  return radio.isChipConnected();
}

bool Lightbar::isAvailable() { return radio.isChipConnected(); }

// CRC-16 over `data`: poly 0x1021, init 0xFFFE, no input/output reflection,
// no final xor (reverse-engineered Telink CRC, xiaomi_lightbar/baseband.py).
uint16_t Lightbar::crc16(const uint8_t *data, size_t len) {
  uint16_t crc = 0xFFFE;
  for (size_t i = 0; i < len; i++) {
    crc ^= (uint16_t)data[i] << 8;
    for (uint8_t bit = 0; bit < 8; bit++) {
      if (crc & 0x8000) {
        crc = (crc << 1) ^ 0x1021;
      } else {
        crc <<= 1;
      }
    }
  }
  return crc;
}

// Build a 17-byte packet (big-endian):
//   preamble[8] | id[3] | 0xFF | counter[1] | command[2] | crc16[2]
// Note: the counter comes BEFORE the command (matches the code + test vector,
// not the header comment in baseband.py).
void Lightbar::buildPacket(uint8_t out[17], uint16_t command, uint8_t counter) {
  size_t i = 0;
  memcpy(out, PREAMBLE, 8);
  i += 8;
  out[i++] = (id >> 16) & 0xFF;
  out[i++] = (id >> 8) & 0xFF;
  out[i++] = id & 0xFF;
  out[i++] = SEPARATOR;
  out[i++] = counter;
  out[i++] = (command >> 8) & 0xFF;
  out[i++] = command & 0xFF;

  uint16_t crc = crc16(out, i); // over the first 15 bytes
  out[i++] = (crc >> 8) & 0xFF;
  out[i++] = crc & 0xFF;
}

void Lightbar::send(uint16_t code, int counter) {
  uint8_t seq;
  if (counter < 0) {
    seq = this->counter++; // wraps naturally at 256 (uint8_t)
  } else {
    seq = (uint8_t)counter;
  }

  uint8_t pkt[17];
  buildPacket(pkt, code, seq);

  for (uint8_t r = 0; r < repetitions; r++) {
    radio.write(pkt, sizeof(pkt));
    delay(delayMs);
  }
}

void Lightbar::on_off(int counter) { send(0x0100, counter); }

String Lightbar::packetHex(uint16_t command, uint8_t counter) {
  uint8_t pkt[17];
  buildPacket(pkt, command, counter);
  String s;
  for (size_t i = 0; i < sizeof(pkt); i++) {
    if (pkt[i] < 0x10) {
      s += '0';
    }
    s += String(pkt[i], HEX);
  }
  s.toUpperCase();
  return s;
}
