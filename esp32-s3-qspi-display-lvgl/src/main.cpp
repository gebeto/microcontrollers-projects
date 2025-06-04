#include <Arduino_GFX_Library.h>
#include "lvgl.h"

#define PIN_LCD_CS (GPIO_NUM_12)
#define PIN_LCD_PCLK (GPIO_NUM_10)
#define PIN_LCD_DATA0 (GPIO_NUM_13)
#define PIN_LCD_DATA1 (GPIO_NUM_11)
#define PIN_LCD_DATA2 (GPIO_NUM_14)
#define PIN_LCD_DATA3 (GPIO_NUM_9)
#define PIN_LCD_RST (GPIO_NUM_8)
#define PIN_BK_LIGHT (GPIO_NUM_17)

Arduino_DataBus *bus;
Arduino_GFX *gfx;

static uint32_t screenWidth = 472;
static uint32_t screenHeight = 466;
lv_disp_draw_buf_t draw_buf;
lv_color_t *disp_draw_buf1;
lv_color_t *disp_draw_buf2;
lv_disp_drv_t disp_drv;

/* Display flushing */
void display_driver_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);

  lv_disp_flush_ready(disp);
}

void lcd_init()
{
  bus = new Arduino_ESP32QSPI(PIN_LCD_CS, PIN_LCD_PCLK, PIN_LCD_DATA0, PIN_LCD_DATA1, PIN_LCD_DATA2, PIN_LCD_DATA3, false);
  gfx = new Arduino_CO5300(bus, PIN_LCD_RST, 0, false, screenWidth, screenHeight);

  pinMode(PIN_BK_LIGHT, OUTPUT);
  digitalWrite(PIN_BK_LIGHT, HIGH);

  gfx->begin(80000000);

  disp_draw_buf1 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * screenHeight / 8, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  disp_draw_buf2 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * screenHeight / 8, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf1, disp_draw_buf2, screenWidth * screenHeight / 8);

  /* Initialize the display */
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = display_driver_flush_cb;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);
}

void setup()
{
  lv_init();
  lcd_init();

  lv_disp_t *disp = lv_disp_get_default();
  lv_obj_t *screen = lv_obj_create(NULL);

  lv_obj_t *spinner = lv_spinner_create(screen, 1000, 60);
  lv_obj_set_size(spinner, 400, 400);
  lv_obj_center(spinner);

  lv_obj_t *label = lv_label_create(screen);
  lv_obj_set_width(label, LV_SIZE_CONTENT);
  lv_obj_set_height(label, LV_SIZE_CONTENT);
  lv_obj_set_x(label, 0);
  lv_obj_set_y(label, 0);
  lv_obj_set_align(label, LV_ALIGN_CENTER);
  lv_label_set_text(label, "Hello world!");
  lv_obj_set_style_text_font(label, &lv_font_montserrat_48, LV_STATE_DEFAULT);

  lv_disp_load_scr(screen);
}

void loop()
{
  lv_timer_handler(); // Call LVGL's timer handler to process events
  delay(20);          // Delay to allow for processing time
}
