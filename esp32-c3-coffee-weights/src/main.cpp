
#include <U8g2lib.h>
#include <string>
#include <stdio.h>

#include "CoffeeScale.h"
#include "Screen.h"

#define DOUT_PIN 0
#define SCK_PIN 8
CoffeeScale scale = CoffeeScale(DOUT_PIN, SCK_PIN);
TaskHandle_t weightTaskHandle;
void weightsTask(void *pvParameters)
{
  scale.start();

  for (;;)
  {
    scale.readWeight();
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

#define SCL_PIN 7
#define SDA_PIN 6
TaskHandle_t screenTaskHandle;
Screen screen = Screen(SCL_PIN, SDA_PIN);

void screenTask(void *pvParameters)
{
  screen.start();
  for (;;)
  {
    screen.draw(scale.getWeightLabel());
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

void setup(void)
{
  Serial.begin(9600);

  xTaskCreatePinnedToCore(weightsTask, "weightsTask", 2048, NULL, 1, &weightTaskHandle, 0);
  xTaskCreatePinnedToCore(screenTask, "screenTask", 2048, NULL, 1, &screenTaskHandle, 0);
}

void loop(void) {}
