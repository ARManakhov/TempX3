
#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include "Sensor.h"
#include <vector>

class Settings
{
private:
    std::vector<Sensor*> *sensors;
    bool zoomerMuted;
    bool zoomerInverted;
    byte brightness;
    int * maxTemps;
    int * minTemps;
public:

};

#endif