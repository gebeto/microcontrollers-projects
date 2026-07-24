#include "HomeSpan.h"



struct LED : Service::LightBulb
{
  int ledPin;
  Lightbar lightbar;
  SpanCharacteristic *power;

  LED(int ledPin) : Service::LightBulb()
  {
    this->lightbar = new Lightbar(NRF_CE_PIN, NRF_CSN_PIN, REMOTE_ID)
    power = new Characteristic::On();
    this->ledPin = ledPin;
    pinMode(ledPin, OUTPUT);
  }

  boolean update()
  {
    digitalWrite(ledPin, !power->getNewVal());
    return (true);
  }
};
