#include <Arduino.h>
#include <vector>
#include "Sensor.h"
#include "Settings.h"

class SensorsManager
{
private:
    std::vector<Sensor *> *sensorsInUse;
    std::vector<Sensor *> *sensorsAll;
    Settings *settings;
    OneWire *oneWire;
    bool scanAllNew();
    long scanCooldown = 0;
public:
    SensorsManager(std::vector<Sensor *> *sensors, Settings *settings);
    void dispatch();
};