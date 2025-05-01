#include <lvgl.h>


lv_obj_t *page2;


lv_obj_t* page_2_create(void) {
  if (page2 != NULL) {
    lv_obj_del(page2);
  }

  page2 = lv_obj_create(NULL);
  lv_obj_t *label1 = lv_label_create(page2);
  lv_label_set_text(label1, "This is Page Test 2\nSHIIIISH");
  lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);

  // lv_scr_load(page2);
  return page2;
}


void page_2_destroy(void) {
  lv_obj_del(page2);
  page2 = NULL;  
}

