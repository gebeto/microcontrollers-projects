#include <lvgl.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

// === CONFIG ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define I2C_SDA 8
#define I2C_SCL 9

#define LVGL_TICK_PERIOD 5  // ms

// === DISPLAY DRIVER ===
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// === LVGL BUFFER ===
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[SCREEN_WIDTH * SCREEN_HEIGHT / 8];  // 1-bit display

// === FLUSH CALLBACK ===
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  display.clearDisplay();

  for (int y = area->y1; y <= area->y2; y++) {
    for (int x = area->x1; x <= area->x2; x++) {
      int index = x + y * SCREEN_WIDTH;
      if (color_p[index].full) {
        display.drawPixel(x, y, SSD1306_WHITE);
      } else {
        display.drawPixel(x, y, SSD1306_BLACK);
      }
    }
  }

  display.display();  // Push to screen
  lv_disp_flush_ready(disp);
}


void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 init failed"));
    while (true);
  }

  display.clearDisplay();
  display.display();

  // === LVGL INIT ===
  lv_init();

  lv_disp_draw_buf_init(&draw_buf, buf1, NULL, SCREEN_WIDTH * SCREEN_HEIGHT / 8);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.hor_res = SCREEN_WIDTH;
  disp_drv.ver_res = SCREEN_HEIGHT;
  lv_disp_drv_register(&disp_drv);

  // === SIMPLE UI ===
  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Hello SSD1306 + LVGL!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

void loop() {
  lv_timer_handler();  // Refresh UI
  delay(1);
}