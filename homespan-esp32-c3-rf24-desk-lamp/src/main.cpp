#include "HomeSpan.h"
#include "LED.h"

#include <Wire.h>
#include "Lightbar.h"
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>

// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

#define REMOTE_ID 0x45b510
#define NRF_CE_PIN 10
#define NRF_CSN_PIN 7
#define NRF_SCK_PIN 4
#define NRF_MISO_PIN 5
#define NRF_MOSI_PIN 6

void setup() {
  // put your setup code here, to run once:
  // Serial.begin(115200);
  Serial.begin(9600);
  if (lightbar.begin()) {
    Serial.println("nRF24 chip connected");
  } else {
    Serial.println("WARNING: nRF24 chip NOT responding -- check wiring/power");
  }

  homeSpan.setPairingCode("11122333");

  homeSpan.begin(Category::Lighting, "Desk Lamp");

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
    new LED(8);
  
    // if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    //   Serial.println(F("SSD1306 allocation failed"));
    //   for(;;); // Don't proceed, loop forever
    // }
  
    // display.clearDisplay();
    // display.setTextColor(WHITE);
    // display.setTextSize(1);
    // display.setCursor(0, 0);
    // display.println("Hello, world!");
    // display.display();
}


void loop() {
  // put your main code here, to run repeatedly:
  homeSpan.poll();
}

