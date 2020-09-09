#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <OneWire.h>

class Sensor
{
public:
    Sensor(byte addr[8],OneWire * oneWire);
    void readData();
    float getTemp();
    float getLastTemp();
    byte * getAddress();
    void setScreenOrder(byte order);
    byte getScreenOrder();
    void setEepromOrder(byte order);
    byte getEepromOrder();
    void setExpireOrder(byte order);
    byte getExpireOrder();
    void init();
    bool isDisconnected();
    bool equalAddr(Sensor * sensor);
    bool equalAddr(byte * addr);
private:
    byte screenOrder;
    byte eepromOrder;
    byte expireOrder;
    bool disconnected = true;
    bool scanned = false;
    byte * addr;
    byte lastData[12];
    OneWire * oneWire;
    byte present = 0;
    float lastTemp;
};

#endif 
