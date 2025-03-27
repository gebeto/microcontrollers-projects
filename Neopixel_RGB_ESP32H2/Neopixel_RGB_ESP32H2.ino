#include <Adafruit_NeoPixel.h>

#define LED_PIN 8      // GPIO8 where the LED is connected
#define NUM_LEDS 1     // Only one built-in LED
#define DELAY 100

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void loop() {
    setColor(255, 0, 0);  // Red
    delay(DELAY);
    setColor(0, 255, 0);  // Green
    delay(DELAY); 
    setColor(0, 0, 255);  // Blue
    delay(DELAY);
    setColor(255, 255, 255);  // White
    delay(DELAY);
}

void setColor(uint8_t r, uint8_t g, uint8_t b) {
    strip.setPixelColor(0, strip.Color(r, g, b));
    strip.show();
}