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
    Sensor * firstSensor;
    Sensor * lastSensor;
    bool sensorsInRange[screen_ls] = {false};
    bool beeperMuted[2];
    bool beeperInverted[2];
    byte brightness[2];
    byte *addresses[screen_ls];
    int16_t maxTemps[screen_ls * 2];
    int16_t minTemps[screen_ls * 2];
    bool saveBooleans();
    bool saveNewSensor(Sensor * sensor);
    bool saveExistSensor(Sensor * sensor, Sensor * secondSensor);
    bool saveMaxTemps();
    bool saveMinTemps();
    bool saveBrightness();
    bool readBooleans();
    bool readAddresses();
    bool readMaxTemps();
    bool readMinTemps();


private:
    bool readBrightness();
    bool makeFirst(Sensor * sensor);
    byte getSensorOrder(Sensor sensor);
    Screen *screen;
    OneWire *oneWire;

public:
    bool *getSensorsInRange();

    Settings(Screen *screen, OneWire *oneWire, std::vector<Sensor *> *sensorsFromEeprom);
    bool *getBeeperMute();
    bool commitBeeperMute();

    bool *getBeeperInverted();
    bool commitBeeperInverted();

    byte *getBrightness();
    bool commitBrightness();

    int16_t *getMaxTemps();
    bool commitMaxTemps();

    int16_t *getMinTemps();
    bool commitMinTemps();

    bool updateSensorsData(Sensor * sensor);
};

#endif