
#include <M5Unified.h>
#include <M5GFX.h>
#include "TicTacToe.cpp"

void drawGrid();

m5::touch_detail_t touchDetail;
m5::touch_detail_t pressedDetails;

static bool isStateChanged = true;

TicTacToe game;

void setup(void)
{
    M5.begin();
    M5.Display.startWrite();
    M5.Display.setRotation(0);

    game.init();

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
        game.recalculateHighlighted(pressedDetails);
        drawGrid();
        isStateChanged = false;
    }
}

void drawGrid()
{
    M5.Display.waitDisplay();

    for (int index = 0; index < 9; index++)
    {
        GridCell rect = game.getCell(index);

        if (index == game.highlightedIndex)
        {
            M5.Display.fillRect(rect.x, rect.y, cellSize, cellSize, color);
        }
        else
        {
            M5.Display.fillRect(rect.x, rect.y, cellSize, cellSize, emptyColor);
        }
        M5.Display.drawRect(rect.x, rect.y, cellSize, cellSize, color);
    }

    Serial.println("Render Screen!");

    M5.Display.display();
}