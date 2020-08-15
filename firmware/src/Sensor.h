//
// Created by sirosh on 23.06.2020.
//

#ifndef UNTITLED3_SENSOR_H
#define UNTITLED3_SENSOR_H

#include <Arduino.h>
#include <OneWire.h>

class Sensor
{
public:
    Sensor(byte *addr, const OneWire &oneWire);
    Sensor(byte *addr, uint8_t pin);
    void readData();
    float getTemp();
    float getLastTemp();
    byte *getAddress();
    void init();
    bool isDisconnected();

private:
    bool disconnected = true;
    bool scanned = false;
    byte *addr;
    byte lastData[12];
    OneWire oneWire;
    byte present = 0;
    float lastTemp;
};

#endif //UNTITLED3_SENSOR_H
