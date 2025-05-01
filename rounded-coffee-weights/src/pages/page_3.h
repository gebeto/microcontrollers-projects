#include <lvgl.h>


lv_obj_t *page3;


lv_obj_t* page_3_create(void) {
  if (page3 != NULL) {
    lv_obj_del(page3);
  }

  page3 = lv_obj_create(NULL);
  lv_obj_t *label1 = lv_label_create(page3);
  lv_label_set_text(label1, "This is Page Test 3\nGOOOD");
  lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);

  return page3;
}


void page_3_destroy(void) {
  lv_obj_del(page3);
  page3 = NULL;  
}

