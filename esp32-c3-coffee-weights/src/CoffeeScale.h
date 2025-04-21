#include <string>
#include <stdio.h>

#include "HX711.h"


class CoffeeScale
{

private:
    int dout_pin;
    int sck_pin;
    HX711 scale;

    long weight = 0;

public:
    CoffeeScale(int DOUT_PIN, int SCK_PIN)
    {
        this->scale = HX711();
        this->dout_pin = DOUT_PIN;
        this->sck_pin = SCK_PIN;
    }

    void start()
    {
        scale.begin(this->dout_pin, this->sck_pin);

        const int LOADCELL_OFFSET = -316365;
        const float LOADCELL_DIVIDER = 102.6936;
        this->scale.set_scale(LOADCELL_DIVIDER);
        this->scale.set_offset(LOADCELL_OFFSET);
        this->scale.wait_ready_timeout(1000);
    }

    long readWeight()
    {
        this->weight = this->scale.get_units(5);
        return this->weight;
    }

    std::string getWeightLabel()
    {
        char weightFloatChars[10];
        float grams = static_cast<float>(this->weight) / 10.0f;
        dtostrf(grams, 4, 1, weightFloatChars);
        std::string _weightStr = weightFloatChars;
        return _weightStr;
    }
};
