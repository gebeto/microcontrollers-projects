#include <iomanip>

class CoffeeTimer
{

private:
    bool started;
    int seconds;

public:
    CoffeeTimer(int initialSeconds = 0)
    {
        this->started = false;
        this->seconds = initialSeconds;
    }

    void start()
    {
        this->started = true;
    }

    void stop()
    {
        this->started = false;
    }

    void reset()
    {
        this->seconds = 0;
        this->started = false;
    }

    void tick()
    {
        if (this->started)
        {
            this->seconds++;
        }
    }

    std::string getLabel()
    {
        int minutes = this->seconds / 60;
        int seconds = this->seconds % 60;
        std::string minutesString = std::to_string(minutes);
        std::string secondsString = std::to_string(seconds);
        std::ostringstream weightLabelStream;
        weightLabelStream
            << std::setw(2) << std::setfill('0') << minutes
            << ":" << std::setw(2) << std::setfill('0') << seconds;
        return weightLabelStream.str();
    }
};
