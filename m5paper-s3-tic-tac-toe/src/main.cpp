
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

struct Point
{
    int x;
    int y;
};

class GridCell
{
public:
    int x;
    int y;
    int width;
    int height;

    GridCell()
    {
        this->x = 0;
        this->y = 0;
        this->width = 0;
        this->height = 0;
    }

    GridCell(int x, int y, int width, int height)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    int getEndX()
    {
        return this->x + this->width;
    }

    int getEndY()
    {
        return this->y + this->height;
    }

    bool pointInRect(int x, int y)
    {
        if (x > this->x && x < this->getEndX() && y > this->y && y < this->getEndY())
        {
            return true;
        }

        return false;
    }

    bool pointInRect(Point point)
    {
        if (point.x > this->x && point.x < this->getEndX() && point.y > this->y && point.y < this->getEndY())
        {
            return true;
        }

        return false;
    }
};

void drawGrid()
{
    M5.Display.waitDisplay();

    GridCell grid[9];
    for (int col = 0, itemsIndex = 0; col < 3; col++)
    {
        for (int row = 0; row < 3; row++)
        {
            int boxX = offset + row * (cellSize + offset);
            int boxY = offset + col * (cellSize + offset);
            int boxEndX = boxX + cellSize;
            int boxEndY = boxY + cellSize;

            grid[itemsIndex] = *new GridCell(boxX, boxY, cellSize, cellSize);
            itemsIndex++;
        }
    }

    for (int col = 0, itemIndex = 0; col < 3; col++)
    {
        for (int row = 0; row < 3; row++, itemIndex++)
        {
            GridCell rect = grid[itemIndex];
            if (rect.pointInRect(pressedDetails.x, pressedDetails.y))
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
            GridCell rect = grid[index];

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

    Serial.println("Render Screen!");

    M5.Display.display();
}