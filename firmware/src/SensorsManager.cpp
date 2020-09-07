#include "SensorsManager.h"
#include "Settings.h"
#include "settingsHW.h"
#include <vector>

void SensorsManager::scanAllNew(std::vector<Sensor *> *sensors)
{
    byte *addr_p;
    addr_p = new byte[8];
    while (oneWire->search(addr_p))
    {
        bool inVector=false;
        for (size_t i = 0; i < sensors->size(); i++)
        {
            if (sensors->at(i)->equalAddr(addr_p))
            {
                inVector =true;
            }
        }
        if (inVector)
        {
            continue;
        }
        
        
        Sensor *newSensor = new Sensor(addr_p, oneWire);
        sensors->push_back(newSensor);
        Serial.print("new sensor ");
        Serial.println(newSensor->getAddress()[7]);
        newSensor->init();
        addr_p = new byte[8];
    }
}

void SensorsManager::dispatch()
{
    bool hasDisconnected = false;
    for (size_t i = 0; i < sensorsInUse->size(); i++)
    {
        sensorsInUse->at(i)->readData();
        sensorsInUse->at(i)->getTemp();
        if(sensorsInUse->at(i)->isDisconnected()){
            hasDisconnected = true;
        }
    }
    if ((sensorsInUse->size() < screen_ls || hasDisconnected) && scanCooldown + 150 < millis())
    {
        scanAllNew(sensorsInUse);  
        scanCooldown = millis();
    }
}

SensorsManager::SensorsManager(std::vector<Sensor *> *sensors, Settings *settings) : sensorsInUse(sensors), settings(settings)
{
    sensorsAll = new std::vector<Sensor *>();
    oneWire = new OneWire(sensors_pin);
}

