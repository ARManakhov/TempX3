#include "SensorsManager.h"
#include "Settings.h"
#include "settingsHW.h"
#include <vector>

bool SensorsManager::scanAllNew()
{
    byte *addr_p;
    addr_p = new byte[8];

    bool isHasNew = false;

    while (oneWire->search(addr_p))
    {
        bool inVector = false;
        for (size_t i = 0; i < sensorsAll->size(); i++)
        {
            if (sensorsAll->at(i)->equalAddr(addr_p))
            {
                inVector = true;
            }
        }

        if (inVector)
        {
            continue;
        }
        else
        {
            isHasNew = true;
        }

        for (size_t i = 0; i < sensorsInUse->size(); i++)
        {
            if (sensorsInUse->at(i)->equalAddr(addr_p))
            {
                inVector = true;
            }
        }

        if (inVector)
        {
            continue;
        }
        else
        {
            isHasNew = true;
        }

        Sensor *newSensor = new Sensor(addr_p, oneWire);
        sensorsAll->push_back(newSensor);
        Serial.print("new sensor ");
        Serial.println(newSensor->getAddress()[7]);
        newSensor->init();
        addr_p = new byte[8];
    }

    return isHasNew;
}

void SensorsManager::dispatch()
{
    bool hasDisconnected = false;
    for (size_t i = 0; i < sensorsInUse->size(); i++)
    {
        sensorsInUse->at(i)->readData();
        sensorsInUse->at(i)->getTemp();
        if (sensorsInUse->at(i)->isDisconnected())
        {
            hasDisconnected = true;
        }
    }
    if ((sensorsInUse->size() < screen_ls || hasDisconnected) && scanCooldown + 150 < millis())
    {
        bool isHasNew = scanAllNew();
        if (isHasNew)
        {
            while (sensorsAll->size() > 0 && sensorsInUse->size() < screen_ls)
            {
                sensorsInUse->push_back(sensorsAll->back());
                sensorsAll->pop_back();
            }

            if (sensorsAll->size() > 0)
            {
                for (size_t i = 0; i < sensorsInUse->size(); i++)
                {
                    if (sensorsInUse->at(i)->isDisconnected())
                    {
                        sensorsInUse->erase(sensorsInUse->begin() + i);
                        sensorsInUse->insert(sensorsInUse->begin() + i , sensorsAll->back());
                        sensorsAll->pop_back();
                    }
                }
            }
        }
        scanCooldown = millis();
    }
}

SensorsManager::SensorsManager(std::vector<Sensor *> *sensors, Settings *settings, OneWire *oneWire) : sensorsInUse(sensors), settings(settings), oneWire(oneWire)
{
    sensorsAll = new std::vector<Sensor *>();
}
