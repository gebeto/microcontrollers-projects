#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <Encoder.h>
#include <Bounce2.h>


lv_obj_t *page1;


lv_obj_t* page_1_create(void) {
  if (page1 != NULL) {
    lv_obj_del(page1);
  }

  page1 = lv_obj_create(NULL);
  lv_obj_t *label1 = lv_label_create(page1);
  lv_label_set_text(label1, "This is Page Test 1\nOH MY GOOOOD");
  lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);
  return page1;
}


void page_1_destroy(void) {
  lv_obj_del(page1);
  page1 = NULL;  
}

