
#include <U8g2lib.h>
#include <string>
#include <stdio.h>


class Screen
{

private:
    U8G2_SSD1309_128X64_NONAME0_1_SW_I2C u8g2;

public:
    Screen(int SCL_PIN, int SDA_PIN) : u8g2(U8G2_R0, SCL_PIN, SDA_PIN)
    {
        
    }

    void start()
    {
        this->u8g2.begin();
        this->u8g2.setFont(u8g2_font_ncenB08_tr);
    }

    void draw(std::string weightLabel)
    {
        this->u8g2.firstPage();
        do
        {
            std::string weightStr = "Weight: " + weightLabel;
            this->u8g2.drawStr(0, 10, weightStr.c_str());
        } while (this->u8g2.nextPage());
    }
};
