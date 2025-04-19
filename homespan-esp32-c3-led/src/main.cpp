// #include <Arduino.h>
#include "HomeSpan.h"
#include "LED.h"

void setup() {
  // put your setup code here, to run once:
  // Serial.begin(115200);
  Serial.begin(9600);

  homeSpan.setPairingCode("11122333");

  homeSpan.begin(Category::Lighting, "My LED");

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
    new LED(8);
}


void loop() {
  // put your main code here, to run repeatedly:
  homeSpan.poll();
}
