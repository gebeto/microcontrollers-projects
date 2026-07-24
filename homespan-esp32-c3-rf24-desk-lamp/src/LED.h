#include "HomeSpan.h"
#include "config.h"
#include "Lightbar.h"

extern Lightbar lightbar; // defined in main.cpp

struct LED : Service::LightBulb
{
  SpanCharacteristic *power;
  boolean isOn = false; // tracks the lamp's commanded state (bar is toggle-only)

  LED() : Service::LightBulb()
  {
    power = new Characteristic::On();
  }

  boolean update()
  {
    boolean requested = power->getNewVal();
    if (requested != isOn) {
      lightbar.on_off(); // bar is toggle-only; only toggle on a real state change
      isOn = requested;
    }
    return (true);
  }
};
