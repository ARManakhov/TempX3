
#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include "Sensor.h"
#include "settingsHW.h"
#include "Screen.h"
#include "OneWire.h"
#include <vector>

class Settings
{
private:
    std::vector<Sensor *> *sensors;
    std::vector<Sensor *> *sensorsFromEEPROM;
    bool zoomerMuted[2];
    bool zoomerInverted[2];
    byte brightness[2];
    byte *addressess[screen_ls];
    int16_t maxTemps[screen_ls * 2];
    int16_t minTemps[screen_ls * 2];
    bool saveBools();
    bool saveNewSensor(Sensor * sensor);
    bool saveMaxTemps();
    bool saveMinTemps();
    bool saveBrightness();
    bool readBools();
    bool readAddresses();
    bool readMaxTemps();
    bool readMinTemps();
    bool readBrightness();
    byte getSensorOrder(Sensor sensor);
    Screen *screen;
    OneWire *oneWire;

public:
    Settings(Screen *screen, OneWire *oneWire);
    bool *getZoomerMute();
    bool commitZoomerMute();

    bool *getZoomerInverted();
    bool commitZoomerInverted();

    byte *getBrightness();
    bool commitBrightness();

    int16_t *getMaxTemps();
    bool commitMaxTemps();

    int16_t *getMinTemps();
    bool commitMinTemps();

    bool updateSensorsData(Sensor * sensor);
};

#endif