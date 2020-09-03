#include <Arduino.h>
#include "Settings.h"
#include "vector"
#include "Screen.h"


class DisplayManager
{
private:
    byte * scState;
    Settings oldSettings;
    Settings newSettings;
    std::vector<Sensor *> *sensors;
    Screen *screen;
public:
    DisplayManager(byte * scState, Settings settings, std::vector<Sensor *> *sensors, Screen *screen);
    ~DisplayManager();
    void dispatch();
};

