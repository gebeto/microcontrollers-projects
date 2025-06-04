#include <Arduino_GFX_Library.h>

#define EXAMPLE_PIN_NUM_LCD_CS (GPIO_NUM_12)
#define EXAMPLE_PIN_NUM_LCD_PCLK (GPIO_NUM_10)
#define EXAMPLE_PIN_NUM_LCD_DATA0 (GPIO_NUM_13)
#define EXAMPLE_PIN_NUM_LCD_DATA1 (GPIO_NUM_11)
#define EXAMPLE_PIN_NUM_LCD_DATA2 (GPIO_NUM_14)
#define EXAMPLE_PIN_NUM_LCD_DATA3 (GPIO_NUM_9)
#define EXAMPLE_PIN_NUM_LCD_RST (GPIO_NUM_8)
#define EXAMPLE_PIN_NUM_BK_LIGHT (GPIO_NUM_17)

Arduino_ESP32QSPI *bus;
Arduino_GFX *display;

void init_LCD() {
  pinMode(EXAMPLE_PIN_NUM_BK_LIGHT, OUTPUT);
  digitalWrite(EXAMPLE_PIN_NUM_BK_LIGHT, HIGH);

  bus = new Arduino_ESP32QSPI(
      EXAMPLE_PIN_NUM_LCD_CS,
      EXAMPLE_PIN_NUM_LCD_PCLK,
      EXAMPLE_PIN_NUM_LCD_DATA0,
      EXAMPLE_PIN_NUM_LCD_DATA1,
      EXAMPLE_PIN_NUM_LCD_DATA2,
      EXAMPLE_PIN_NUM_LCD_DATA3,
      false);
  display = new Arduino_CO5300(bus, EXAMPLE_PIN_NUM_LCD_RST, 0, false, 471, 466);
}

void setup()
{
  // Serial.begin(115200);
  init_LCD();

  display->begin(8000000);
  display->fillScreen(RED);
  display->setCursor(200, 200);
  display->setTextColor(WHITE);
  display->setTextSize(2);
  display->drawRect(0, 0, 200, 200, RED);
  // display->println("GC9A01 QSPI!");
}

int counter = 0;

void loop()
{
  display->setCursor(200, 200);
  display->println("HELLO WORLD");
  counter++;
  if (counter % 2 == 0) {
    display->fillRect(200, 200, 66, 66, BLUE);
  } else {
    display->fillRect(200, 200, 66, 66, GREEN);
  }
  delay(100);
}
