#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <Encoder.h>
#include <Bounce2.h>
#include <ESP32Time.h>
#include "LCD.h"
#include "ui/ui.h"


#define ROTARY_ENCODER_A_PIN 7
#define ROTARY_ENCODER_B_PIN 6


Encoder rotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN);
int32_t encoder_value = 0;

ESP32Time rtc(0); 


// Task handler function
void lvglTask(void *pvParameters) {
  while (true) {
    lv_timer_handler(); // Handle LVGL tasks
    vTaskDelay(pdMS_TO_TICKS(5)); // delay 5ms
  }
}


// Input processing tasks
void inputTask(void *pvParameters) {
  Serial.println("inputTask started");
  int32_t last_encoder_value = 0;

  while (true) {
    int32_t current_encoder_value = rotaryEncoder.read() / 2;
    int32_t encoder_diff = current_encoder_value - last_encoder_value;

    if (encoder_diff != 0) {
      encoder_value += encoder_diff;
      last_encoder_value = current_encoder_value;
      rtc.setTime(0);
    }
    
    char label[32] = "";
    // sprintf(label, "00:%d", encoder_value);
    sprintf(label, rtc.getTime("%M:%S").c_str());
    lv_label_set_text(ui_TimerLabel, label);

    vTaskDelay(pdMS_TO_TICKS(50)); // delay is 50 milliseconds
  }
}


void setup()
{
  // Serial.begin(115200);
  Serial.begin(9600);

  rotaryEncoder.write(0);

  lv_init();
  lcd_init();
  ui_init();

  lv_label_set_text(ui_TimerLabel, "10:00");
  xTaskCreatePinnedToCore(lvglTask, "lvglTask", 8192, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(inputTask, "inputTask", 8192, NULL, 2, NULL, 1);
}

void loop() {}

