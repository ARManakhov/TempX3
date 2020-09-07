//
// Created by sirosh on 23.06.2020.
//

#include "Sensor.h"
#include <ArduinoSTL.h>
#include "ds18b20.h"
#include "settingsHW.h"

void Sensor::readData()
{
    oneWire->reset();
    oneWire->select(addr);
    oneWire->write(STARTCONVO, 1); // start conversion, with parasite power on at the end

    delay(sensors_scan_period); // maybe 750ms is enough, maybe not
    // we might do a ds.depower() here, but the reset will take care of it.

    oneWire->reset();
    oneWire->select(addr);
    oneWire->write(READSCRATCH, 1); // Read Scratchpad

    delay(sensor_before_read_period);

    for (int i = 0; i < 9; i++)
    { // we need 9 bytes
        lastData[i] = oneWire->read();
    }
}

float Sensor::getTemp()
{
    scanned = true;
    int16_t raw = (lastData[1] << 8) | lastData[0];

    byte cfg = (lastData[4] & 0x60);
    if (cfg == 0x00)
    {
        //Serial.print("res 9");
        raw = raw & ~7; // 9 bit resolution, 93.75 ms
        disconnected = false;
    }
    else if (cfg == 0x20)
    {

        //Serial.print("res 10");
        raw = raw & ~3; // 10 bit res, 187.5 ms
        disconnected = false;
    }
    else if (cfg == 0x40)
    {

        //Serial.print("res 11");
        raw = raw & ~1; // 11 bit res, 375 ms
        disconnected = false;
    }else{
        disconnected = true;
    }

    lastTemp = (float)raw / 16.0;
    return lastTemp;
}

float Sensor::getLastTemp()
{
    if (!scanned)
    {
        return 0;
    }
    return lastTemp;
}

byte *Sensor::getAddress()
{
    return addr;
}

Sensor::Sensor(byte addr[8], OneWire * oneWire) : addr(addr), oneWire(oneWire)
{
}

bool Sensor::isDisconnected(){
    return disconnected;
}

void Sensor::init()
{
    oneWire->reset();
    oneWire->select(addr);
    oneWire->write(WRITESCRATCH, 1);         

    delay(100);
    oneWire->write(0);
    oneWire->write(0);
    oneWire->write(TEMP_9_BIT);

    oneWire->reset();
    oneWire->select(addr);
    oneWire->write(COPYSCRATCH, 1);

    
}

bool Sensor::equalAddr(Sensor * sensor){
    byte * sensorAddress = sensor->getAddress();
    for (size_t i = 0; i < 8; i++)
    {
        if (sensorAddress[i] != addr[i])
        {
            return false;
        }
    }
    return true;

}

bool Sensor::equalAddr(byte * sensorAddress){
    for (size_t i = 0; i < 8; i++)
    {
        if (sensorAddress[i] != addr[i])
        {
            return false;
        }
    }
    return true;

}

