#include <U8g2lib.h>
#include <string>
#include <stdio.h>

#include "CoffeeScale.h"
#include "Screen.h"
#include "CoffeeTimer.h"

// #include <Arduino.h>
// #include <Buzzer.h>
// #define BUZZER_PIN 2

// Buzzer::Melody_t myMelody{.nbNotes = 3,
//                               .duration = {400, 400, 800},
//                               .frequency = {D4_NOTE_FREQ, E4_NOTE_FREQ, G4_NOTE_FREQ}};
// Buzzer myBuzzer;
// TaskHandle_t beeperTaskHandle;

// void beeperTask(void *parameters)
// {
//   myBuzzer.init(BUZZER_PIN);
//   myBuzzer.setMelody(&myMelody);

//   for (;;)
//   {
//     if (myBuzzer.hasMelody())
//     {
//       Serial.println("I print while buzzer plays!");
//     }

//     myBuzzer.step();
//     vTaskDelay(1000 / portTICK_PERIOD_MS);
//   }
// }

CoffeeTimer coffeeTimer = CoffeeTimer(0);
TaskHandle_t timerTaskHandle;
void timerTask(void *pvParameters)
{
  coffeeTimer.start();
  for (;;)
  {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    coffeeTimer.tick();
  }
}

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
    screen.draw(scale.getWeightLabel(), coffeeTimer.getLabel());
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void setup(void)
{
  // Serial.begin(9600);
  Serial.begin(115200);

  // xTaskCreatePinnedToCore(beeperTask, "beeperTask", 2048, NULL, 1, &beeperTaskHandle, 0);
  xTaskCreatePinnedToCore(timerTask, "timerTask", 2048, NULL, 1, &timerTaskHandle, 0);
  xTaskCreatePinnedToCore(weightsTask, "weightsTask", 2048, NULL, 1, &weightTaskHandle, 0);
  xTaskCreatePinnedToCore(screenTask, "screenTask", 2048, NULL, 1, &screenTaskHandle, 0);
}

void loop(void) {}
