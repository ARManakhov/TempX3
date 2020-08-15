//
// Created by sirosh on 23.06.2020.
//

#ifndef UNTITLED3_SENSORUTIL_H
#define UNTITLED3_SENSORUTIL_H

#include "Sensor.h"
#include <OneWire.h>
#include <vector>

class SensorUtil
{
public:
    void scanAllNew(std::vector<Sensor *> *sensors, OneWire oneWire);
};

#endif //UNTITLED3_SENSORUTIL_H
