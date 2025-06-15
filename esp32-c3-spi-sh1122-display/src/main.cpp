#include <SPI.h>

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>


U8G2_SH1122_256X64_F_4W_SW_SPI u8g2(U8G2_R0, 
  /* clock=*/ 9,
  /* data=*/ 8,
  /* cs=*/ 2,
  /* dc=*/ 3,
  /* reset=*/ 4
);


// task to render screen
int x = 0;
void renderScreen(void *pvParameters) {
  for(;;) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_10x20_me);
    char message[20];
    sprintf(message, "Hello: %d", x);
    u8g2.drawStr(x, 20, message);
    u8g2.sendBuffer();
    x += 1;
    vTaskDelay(pdMS_TO_TICKS(60));
  }
}


void setup(void) {
  Serial.begin(9600);
  u8g2.begin();

  // start task
  xTaskCreate(
    renderScreen, /* Task function. */
    "Render",     /* name of task. */
    10000,        /* Stack size of task */
    NULL,         /* parameter of the task */
    2,            /* priority of the task */
    NULL);        /* Task handle to keep track of created task */
}

void loop() {}