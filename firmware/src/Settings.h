
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
    int16_t maxTemps[screen_ls*2];
    int16_t minTemps[screen_ls*2];
    bool saveBools();
    bool saveAddresses();
    bool saveMaxTemps();
    bool saveMinTemps();
    bool saveBrightness();
    
    bool readBools();
    bool readAddresses();
    bool readMaxTemps();
    bool readMinTemps();
    bool readBrightness();
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

    int16_t * getMaxTemps();
    bool commitMaxTemps();

    int16_t * getMinTemps();
    bool commitMinTemps();
};

#endif