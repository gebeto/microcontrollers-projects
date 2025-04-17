#include <Adafruit_NeoPixel.h> 

#define PIN_NEOPIXEL 14
#define DELAY 25
#define PIXELS 64

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, PIN_NEOPIXEL);

void setup() {
 Serial.begin(115200);
 strip.setBrightness(RGB_BRIGHTNESS / 6);
 strip.begin();
 strip.show(); // Initialize all pixels to 'off' only run once, it runs right when you turn it on
}


void loop() {
  const int length = 6;
  for(int i = 0; i < PIXELS + length / 2; i++){
    const int index = i % PIXELS;
    strip.setPixelColor(i - length / 2, 0, 0, 0, 0);
    strip.setPixelColor(index, random(255), random(255), random(255), 0);
    strip.show();
    delay(DELAY);
  }
  // for(int i = strip.numPixels() - 1; i >= 0; i--){
  //   strip.setPixelColor(i, 0, 0, 0, 0);
  //   strip.show();
  //   delay(50);
  // }
}

