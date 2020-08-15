//
// Created by sirosh on 23.06.2020.
//

#include "SensorUtil.h"
#include <vector>


void SensorUtil::scanAllNew(std::vector<Sensor *> *sensors, OneWire oneWire)
{
    byte *addr_p;
    addr_p = new byte[8];
    while (oneWire.search(addr_p))
    {
        Sensor *newSensor = new Sensor(addr_p, oneWire);
        sensors->push_back(newSensor);
        Serial.print("in util ");
        Serial.println(newSensor->getAddress()[7]);
        newSensor->init();
        addr_p = new byte[8];
    }
}
