#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <Encoder.h>
#include <Bounce2.h>

#include "ui/ui.h"

#define GFX_BL 8

Arduino_DataBus *bus = new Arduino_ESP32SPI(4 /* DC */, 10 /* CS */, 1 /* SCK */, 0 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, GFX_NOT_DEFINED /* RST */, LV_DISP_ROT_270 /* rotation */, true /* IPS */);

#define ROTARY_ENCODER_A_PIN 7
#define ROTARY_ENCODER_B_PIN 6

Encoder rotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN);
int32_t encoder_value = 0;

static uint32_t screenWidth = 240;
static uint32_t screenHeight = 240;
lv_disp_draw_buf_t draw_buf;
lv_color_t *disp_draw_buf1;
lv_color_t *disp_draw_buf2;
lv_disp_drv_t disp_drv;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
  gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

  lv_disp_flush_ready(disp);
}

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
    }
    
    char label[32] = "";
    sprintf(label, "00:%d", encoder_value);
    lv_label_set_text(ui_TimerLabel, label);

    vTaskDelay(pdMS_TO_TICKS(50)); // delay is 50 milliseconds
  }
}


void setup()
{
  // Serial.begin(115200);
  Serial.begin(9600);
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, LOW);
  gfx->begin(80000000);

  rotaryEncoder.write(0);

  lv_init();
  delay(10);

  disp_draw_buf1 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * screenHeight / 8, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  disp_draw_buf2 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * screenHeight / 8, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf1, disp_draw_buf2, screenWidth * screenHeight / 8);

  /* Initialize the display */
  lv_disp_drv_init(&disp_drv);
  /* Change the following line to your display resolution */
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  ui_init();
  lv_label_set_text(ui_TimerLabel, "10:00");
     
  xTaskCreatePinnedToCore(lvglTask, "lvglTask", 8192, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(inputTask, "inputTask", 8192, NULL, 2, NULL, 1);
}

void loop() {}

