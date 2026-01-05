
#include <M5GFX.h>
M5GFX display;

// #include <M5UnitOLED.h>
// M5UnitOLED display; // default setting
// M5UnitOLED display ( 21, 22, 400000 ); // SDA, SCL, FREQ

// #include <M5UnitLCD.h>
// M5UnitLCD display;  // default setting
// M5UnitLCD display  ( 21, 22, 400000 ); // SDA, SCL, FREQ

// #include <M5UnitGLASS2.h>
// M5UnitGLASS2 display;  // default setting
// M5UnitGLASS2 display ( 21, 22, 400000 ); // SDA, SCL, FREQ

// #include <M5AtomDisplay.h>
// M5AtomDisplay display;

static constexpr size_t BAR_COUNT = 64;
static int max_y[BAR_COUNT];
static int prev_y[BAR_COUNT];
static uint32_t colors[BAR_COUNT];

void setup(void)
{
    display.init();
    display.startWrite();
    display.setRotation(0);

    display.setEpdMode(epd_mode_t::epd_fastest);

    //   if (display.isEPD())
    //   {
    //     display.setEpdMode(epd_mode_t::epd_fastest);
    //   }
    //   if (display.width() < display.height())
    //   {
    //     display.setRotation(display.getRotation() ^ 1);
    //   }
}

void loop(void)
{
    display.waitDisplay();

    static uint32_t color = display.color888(0, 0, 0);
    static uint32_t offset = 15;
    static uint32_t cellSize = 160;

    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            display.drawRect(offset + row * (cellSize + offset), offset + col * (cellSize + offset), cellSize, cellSize, color);
        }
    }

    // display.drawRect(10, 10, 120, 120, color);
    // display.drawRect(10, 10 + 120 + 10, 120, 120, color);
    // display.drawRect(10, 10 + 120 + 10 + 120 + 10, 120, 120, color);

    display.display();
}
