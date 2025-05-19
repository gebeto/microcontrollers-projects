#include "HardwareSerial.h"

#define LED_PIN 22

HardwareSerial serial(2);

int val = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  serial.begin(115200, SERIAL_8N1, 16, 17);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = val ? 0 : 1;
  digitalWrite(LED_PIN, val);

  serial.write("Hello");
  // String message = Serial1.readString();
  // if (message) {
  //   Serial.print("Entered:");
  //   Serial.println(message);
  // }

  delay(300);
}

