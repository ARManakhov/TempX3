
#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include "Sensor.h"
#include "settingsHW.h"
#include <vector>

class Settings
{
private:
    std::vector<Sensor*> *sensors;
    bool zoomerMuted[2];
    bool zoomerInverted[2];
    byte brightness[2];
    byte * addressess[screen_ls];
    int maxTemps[screen_ls*2];
    int minTemps[screen_ls*2];
public:
    Settings();
    bool * getZoomerMute();
    bool commitZoomerMute();

    bool * getZoomerInverted();
    bool commitZoomerInverted();

    byte * getBrightness();
    bool commitBrightness();

    byte * * getAddressess();
    bool  commitAddressess();

    int * getMaxTemps();
    bool commitMaxTemps();

    int * getMinTemps();
    bool commitMinTemps();
};

#endif