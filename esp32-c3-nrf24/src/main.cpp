#include <Arduino.h>
#include <stdint.h>
#include <CRC16.h> // robtillaart/CRC library

// Structure of a packet (17 bytes):
// - preamble (8 bytes), common to all devices, 0x533914DD1C493412
// - remote id (3 bytes), hardcoded in the remote
// - separator (1 byte), common to all devices 0xFF
// - command id (2 bytes)
// - sequence counter (1 byte)
// - CRC16 (2 bytes)

// const uint64_t PREAMBLE = 0x533914DD1C493412ULL; // 8 bytes
const uint64_t PREAMBLE = 0x533914DD1C493412; // 8 bytes
const uint8_t SEPARATOR = 0xFF;

void build_packet(uint32_t id, uint16_t command, uint8_t counter, uint8_t* packet) {
    // Preamble (8 bytes, big endian)
    for (int i = 0; i < 8; ++i) {
        packet[i] = (PREAMBLE >> (56 - 8 * i)) & 0xFF;
    }
    // Remote ID (3 bytes, big endian)
    packet[8] = (id >> 16) & 0xFF;
    packet[9] = (id >> 8) & 0xFF;
    packet[10] = id & 0xFF;
    // Separator (1 byte)
    packet[11] = SEPARATOR;
    // Counter (1 byte)
    packet[12] = counter;
    // Command (2 bytes, big endian)
    // packet[13] = (command >> 8) & 0xFF;
    // packet[14] = command & 0xFF;
    packet[13] = 0x01 & 0xFF;
    packet[14] = 0x00 & 0xFF;
    // CRC16 (2 bytes, big endian)
    CRC16 crc;
    crc.reset();
    crc.setPolynome(0x1021);
    crc.setInitial(0xFFFE);
    crc.setXorOut(0x0000);
    crc.setReverseIn(false);
    crc.setReverseOut(false);
    for (int i = 0; i < 15; i++) {
        crc.add(packet[i]);
    }
    uint16_t crcval = crc.getCRC();
    Serial.printf("%d ", crc.count());
    packet[15] = (crcval >> 8) & 0xFF;
    packet[16] = crcval & 0xFF;
}

void print_packet(const uint8_t* packet, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        Serial.printf("%02X ", packet[i]);
    }
    Serial.println();
}

void setup() {
    Serial.begin(9600);
    delay(1000);
    uint8_t packet[17];
    uint32_t remote_id = 0x5421FE; // Example remote id
    uint16_t command = 0x0100;     // Example command
    uint8_t counter = 0x01;        // Example counter
    build_packet(remote_id, command, counter, packet);
    Serial.print("Packet: ");
    print_packet(packet, 17);
}

void loop() {
    // nothing
}