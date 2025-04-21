
#include <U8g2lib.h>
#include <string>
#include <stdio.h>

#include "HX711.h"


// HX711 circuit wiring
#define LOADCELL_DOUT_PIN 0
#define LOADCELL_SCK_PIN 8
#define LOADCELL_OFFSET -316365
#define LOADCELL_DIVIDER 102.6936

HX711 scale;
TaskHandle_t weightTaskHandle;
long weight = 0;
float weightLabel = 0;
long offset = 0;
long initialWeight = 0;


void weightsTask(void *pvParameters) {
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(LOADCELL_DIVIDER);
  scale.set_offset(LOADCELL_OFFSET);
  scale.wait_ready_timeout(1000);

  for(;;) {
    weight = scale.get_units(5);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}


// Screen
#define SCL_PIN 7
#define SDA_PIN 6
TaskHandle_t screenTaskHandle;
U8G2_SSD1309_128X64_NONAME0_1_SW_I2C u8g2(U8G2_R0, SCL_PIN, SDA_PIN);
int counter = 0;

void screenTask(void *pvParameters) {
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  for(;;) {
    u8g2.firstPage();
    char weightFloatChars[10];
    float grams = static_cast<float>(weight) / 10.0f;
    dtostrf(grams, 4, 1, weightFloatChars);
    std::string _weightStr = weightFloatChars;
    do {
      std::string weightStr = "Weight: " + _weightStr;
      u8g2.drawStr(0,10, weightStr.c_str());
      std::string offsetStr = "Offset: " + std::to_string(offset);
      u8g2.drawStr(0,20, offsetStr.c_str());
      std::string initialStr = "Init wei: " + std::to_string(initialWeight);
      u8g2.drawStr(0,30, initialStr.c_str());
      counter ++;
    } while ( u8g2.nextPage() );
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}


void setup(void) {
  Serial.begin(9600);

  xTaskCreatePinnedToCore(weightsTask, "weightsTask", 2048, NULL, 1, &weightTaskHandle, 0);
  xTaskCreatePinnedToCore(screenTask, "screenTask", 2048, NULL, 1, &screenTaskHandle, 0);
}

void loop(void) {}
