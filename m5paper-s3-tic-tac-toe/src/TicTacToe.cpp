
#include <M5Unified.h>
#include <M5GFX.h>

static uint32_t color = M5.Display.color888(0, 0, 0);
static uint32_t emptyColor = M5.Display.color888(255, 255, 255);
static uint32_t offset = 15;
static uint32_t cellSize = 160;

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

class TicTacToe
{
public:
    std::vector<GridCell> cells = {};

    TicTacToe()
    {
        this->cells = {};
    }

    void init()
    {
        for (int col = 0; col < 3; col++)
        {
            for (int row = 0; row < 3; row++)
            {
                int boxX = offset + row * (cellSize + offset);
                int boxY = offset + col * (cellSize + offset);
                // int boxEndX = boxX + cellSize;
                // int boxEndY = boxY + cellSize;

                this->cells.push_back(GridCell(boxX, boxY, cellSize, cellSize));
            }
        }
    }

    GridCell getCell(int index)
    {
        return this->cells[index];
    }
};
