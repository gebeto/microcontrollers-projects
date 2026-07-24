#include "HomeSpan.h"
#include "config.h"
#include "Lightbar.h"

extern Lightbar lightbar; // defined in main.cpp

struct LED : Service::LightBulb
{
  SpanCharacteristic *power;

  LED() : Service::LightBulb()
  {
    power = new Characteristic::On();
  }

  boolean update()
  {
    lightbar.on_off(); // bar is toggle-only; update() fires only on change
    return (true);
  }
};
