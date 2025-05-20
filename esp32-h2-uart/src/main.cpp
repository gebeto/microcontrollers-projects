#include "Arduino.h"
#include "HardwareSerial.h"

// HardwareSerial Serial1(1);

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 1, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available()) {
    String c = Serial1.readStringUntil('\n');
    Serial.println(c);
  }
  delay(100);
}

