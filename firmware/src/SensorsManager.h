#ifndef SENSORS_MANAGE_H
#define SENSORS_MANAGE_H

#include <Arduino.h>
#include <vector>
#include "Sensor.h"
#include "Settings.h"

class SensorsManager
{
private:
    std::vector<Sensor *> *sensorsInUse;
    std::vector<Sensor *> *sensorsAll;
    std::vector<Sensor *> *sensorsFromEeprom;
    Settings *settings;
    OneWire *oneWire;
    bool scanAllNew();
    long scanCooldown = 0;

public:
    SensorsManager(std::vector<Sensor *> *sensors, std::vector<Sensor *> *sensorsFromEeprom, Settings *settings, OneWire *oneWire);
    void dispatch();
};

#endif