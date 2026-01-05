
#include <M5Unified.h>
#include <M5GFX.h>

m5::touch_detail_t touchDetail;

bool pressHandled = false;
m5::touch_detail_t pressedDetails;

static uint32_t color = M5.Display.color888(0, 0, 0);
static uint32_t emptyColor = M5.Display.color888(255, 255, 255);
static uint32_t offset = 15;
static uint32_t cellSize = 160;

static int highlighted = -1;
static bool isStateChanged = true;

void drawGrid();

void setup(void)
{
    M5.begin();
    M5.Display.startWrite();
    M5.Display.setRotation(0);

    M5.Display.setEpdMode(epd_mode_t::epd_fastest);
}

void loop(void)
{
    Serial.begin(115200);
    M5.update();
    touchDetail = M5.Touch.getDetail();

    if (touchDetail.isPressed())
    {
        pressedDetails = touchDetail;
        isStateChanged = true;
    }

    if (isStateChanged && !touchDetail.isPressed())
    {
        drawGrid();
        isStateChanged = false;
    }
}

struct Rect
{
    int x;
    int y;
    int xEnd;
    int yEnd;
};

void drawGrid()
{
    M5.Display.waitDisplay();

    Rect grid[9];
    for (int col = 0, itemsIndex = 0; col < 3; col++)
    {
        for (int row = 0; row < 3; row++)
        {
            int boxX = offset + row * (cellSize + offset);
            int boxY = offset + col * (cellSize + offset);
            int boxEndX = boxX + cellSize;
            int boxEndY = boxY + cellSize;

            grid[itemsIndex] = {boxX, boxY, boxEndX, boxEndY};
            itemsIndex++;
        }
    }

    for (int col = 0, itemIndex = 0; col < 3; col++)
    {
        for (int row = 0; row < 3; row++, itemIndex++)
        {
            Rect rect = grid[itemIndex];
            int deltaX = pressedDetails.x;
            int deltaY = pressedDetails.y;

            if (deltaX > rect.x && deltaX < rect.xEnd && deltaY > rect.y && deltaY < rect.yEnd)
            {
                Serial.printf("Pressed Item! Row: %d, Col: %d \n", row, col);
                highlighted = itemIndex;
            }
        }
    }

    for (int col = 0, index = 0; col < 3; col++)
    {
        for (int row = 0; row < 3; row++)
        {
            Rect rect = grid[index];

            if (index == highlighted)
            {
                M5.Display.fillRect(rect.x, rect.y, cellSize, cellSize, color);
            }
            else
            {
                M5.Display.fillRect(rect.x, rect.y, cellSize, cellSize, emptyColor);
            }
            M5.Display.drawRect(rect.x, rect.y, cellSize, cellSize, color);
            index++;
        }
    }

    M5.Display.display();
}