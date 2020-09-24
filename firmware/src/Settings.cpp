#include "Settings.h"
#include <EEPROM.h>
#include "Screen.h"

byte *Settings::getBrightness()
{
    return brightness;
}

int16_t *Settings::getMaxTemps()
{
    return maxTemps;
}

int16_t *Settings::getMinTemps()
{
    return minTemps;
}

bool *Settings::getBeeperInverted()
{
    return beeperInverted;
}

bool *Settings::getBeeperMute()
{
    return beeperMuted;
}

bool Settings::commitBrightness()
{
    if (brightness[0] != brightness[1])
    {
        brightness[0] = brightness[1];
        saveBrightness();
        screen->setBrigtness(brightness[1]);
        return true;
    }
    return false;
}

bool Settings::commitMaxTemps()
{
    bool wasTrue = false;
    for (byte i = 0; i < screen_ls; i++)
    {
        if (maxTemps[i * 2] != maxTemps[i * 2 + 1])
        {
            wasTrue = true;
            maxTemps[i * 2] = maxTemps[i * 2 + 1];
        }
    }
    if (wasTrue)
    {
        saveMaxTemps();
    }

    return wasTrue;
}

bool Settings::commitMinTemps()
{
    bool wasTrue = false;
    for (byte i = 0; i < screen_ls; i++)
    {
        if (minTemps[i * 2] != minTemps[i * 2 + 1])
        {
            wasTrue = true;
            minTemps[i * 2] = minTemps[i * 2 + 1];
            //write to mem
        }
    }
    if (wasTrue)
    {
        saveMinTemps();
    }

    return wasTrue;
}

bool Settings::commitBeeperInverted()
{
    if (beeperInverted[0] != beeperInverted[1])
    {
        beeperInverted[0] = beeperInverted[1];
        saveBooleans();
        return true;
    }
    return false;
}

bool Settings::commitBeeperMute()
{
    if (beeperMuted[0] != beeperMuted[1])
    {
        beeperMuted[0] = beeperMuted[1];
    }
    return true;
}

bool Settings::saveBooleans()
{
    byte bools = (beeperMuted[0] << 1) + beeperInverted[0];
    EEPROM.write(1, bools);
    return true;
}

bool Settings::saveBrightness()
{
    EEPROM.write(2, brightness[0]);
    return true;
}

bool Settings::saveMinTemps()
{
    Serial.println("saved");
    for (size_t i = 0; i < screen_ls; i++)
    {
        int16_t num = minTemps[i * 2];
        EEPROM.write(3 + i * 2 + 0, num >> 8);
        EEPROM.write(3 + i * 2 + 1, num & 0xFF);
    }
    return true;
}

bool Settings::saveMaxTemps()
{
    Serial.println("saved");
    for (size_t i = 0; i < screen_ls; i++)
    {
        int16_t num = maxTemps[i * 2];
        EEPROM.write(3 + screen_ls * 2 + i * 2 + 0, num >> 8);
        EEPROM.write(3 + screen_ls * 2 + i * 2 + 1, num & 0xFF);
    }
    return true;
}

bool Settings::readAddresses()
{
    for (size_t i = 0; i < screen_ls * 3; i++)
    {
        byte headerInfo = (EEPROM.read(3 + screen_ls * 4 + i * 10)); //first bit say about sensor existance, 2-8 say about order in eeprom
        if (headerInfo >> 7)
        {
            byte screenOrder = headerInfo & 0x01111111;
            byte expireOrder = EEPROM.read(3 + screen_ls * 4 + i * 10 + 1);
            byte addr[8];
            for (size_t j = 0; j < 8; j++)
            {
                addr[j] = EEPROM.read(3 + screen_ls * 4 + i * 10 + 2 + j);
            }
            Sensor *sensor = new Sensor(addr, oneWire);
            sensor->setEepromOrder(i);
            sensor->setScreenOrder(screenOrder);
            sensor->setExpireOrder(expireOrder);
            sensorsFromEEPROM->push_back(sensor);
        }
    }

    if (sensorsFromEEPROM->size() > 0)
    {
        for (size_t i = 0; i < sensorsFromEEPROM->size(); i++)
        {
            Sensor * itertor = sensorsFromEEPROM->at(i);
            if (lastSensor->getExpireOrder() < itertor->getExpireOrder())
            {
                lastSensor = itertor;
            }
            if (itertor->getExpireOrder() == 0)
            {
                firstSensor = itertor;
            }
        }
    }
    return true;
}

bool Settings::updateSensorsData(Sensor *sensor)
{
    if (sensorsFromEEPROM > 0)
    {
        bool alreadyInEeprom = false;
        for (size_t i = 0; i < sensorsFromEEPROM->size(); i++)
        {
            if (sensorsFromEEPROM->at(i)->equalAddr(sensor))
            {
                alreadyInEeprom = true;
                break;
            }
        }
        sensor->setExpireOrder(0);
        if (sensorsFromEEPROM->size() == screen_ls*3)
        {
            sensor->setEepromOrder(lastSensor->getEepromOrder()); 
            firstSensor = sensor;
            lastSensor = NULL;
            for (size_t i = 0; i < sensors->size(); i++)
            {
                if (lastSensor == NULL || sensors->at(i)->getExpireOrder() > lastSensor->getExpireOrder())
                {
                    lastSensor = sensors->at(i);
                }
            }
            saveNewSensor(sensor); 
        } else{
            if (alreadyInEeprom)
            {
                firstSensor->setExpireOrder(1);
                EEPROM.write(3 + screen_ls * 4 + sensor->getEepromOrder() * 10 + 1, sensor->getExpireOrder());
                EEPROM.write(3 + screen_ls * 4 + firstSensor->getEepromOrder() * 10 + 1, firstSensor->getExpireOrder());
                firstSensor = sensor;
            }else{
                byte maxOrder = 0;
                for (size_t i = 0; i < sensors->size(); i++)
                {
                    if (maxOrder < sensors->at(i)->getEepromOrder())
                    {
                        maxOrder = sensors->at(i)->getExpireOrder();
                    }                    
                }
                sensor->setEepromOrder(maxOrder);
                firstSensor = sensor;
                saveNewSensor(firstSensor);
            }
        }
    }
    else
    {
        sensor->setEepromOrder(0);
        sensorsFromEEPROM->push_back(sensor);
        saveNewSensor(sensor);
    }
    return true;
}

bool Settings::saveNewSensor(Sensor *sensor)
{
    byte eepromOrder = sensor->getEepromOrder();
    byte first = 0x10000000 | sensor->getScreenOrder();
    byte second = sensor->getExpireOrder();
    EEPROM.write(3 + screen_ls * 4 + eepromOrder * 10, first);
    EEPROM.write(3 + screen_ls * 4 + eepromOrder * 10 + 1, second);
    byte *address = sensor->getAddress();
    for (size_t i = 0; i < 8; i++)
    {
        EEPROM.write(3 + screen_ls * 4 + sensor->getEepromOrder() * 10 + 2 + i, address[i]);
    }
    for (size_t i = 0; i < sensorsFromEEPROM->size(); i++)
    {
        Sensor *sensorAt = sensorsFromEEPROM->at(i);
        if (!sensorAt->equalAddr(sensor))
        {
            byte newOrder = 1 + sensorAt->getExpireOrder();
            sensorAt->setExpireOrder(newOrder);
            EEPROM.write(3 + screen_ls * 4 + sensorAt->getEepromOrder() * 10 + 1, newOrder);
        }
    }
    return true;
}

bool Settings::readBooleans()
{
    byte bools = EEPROM.read(1);
    beeperInverted[0] = bools & 0xFE;
    beeperInverted[1] = beeperInverted[0];
    beeperMuted[0] = bools >> 1;
    beeperMuted[1] = beeperMuted[0];
    return true;
}

bool Settings::readMaxTemps()
{
    for (size_t i = 0; i < screen_ls; i++)
    {
        maxTemps[i * 2] = (EEPROM.read(3 + screen_ls * 2 + i * 2 + 0) << 8) + EEPROM.read(3 + screen_ls * 2 + i * 2 + 1);
        maxTemps[i * 2 + 1] = maxTemps[i * 2];
    }
    return true;
}

bool Settings::readMinTemps()
{
    for (size_t i = 0; i < screen_ls; i++)
    {
        minTemps[i * 2] = (EEPROM.read(3 + i * 2 + 0) << 8) + EEPROM.read(3 + i * 2 + 1);
        minTemps[i * 2 + 1] = minTemps[i * 2];
    }
    return true;
}

bool Settings::readBrightness()
{
    brightness[0] = EEPROM.read(2);
    brightness[1] = brightness[0];
    screen->setBrigtness(brightness[0]);
    return true;
}

Settings::Settings(Screen *screen, OneWire *oneWire, std::vector<Sensor *> *sensorsFromEeprom) : screen(screen), oneWire(oneWire), sensorsFromEEPROM(sensorsFromEeprom)
{
    //read values to conf from eeprom
    readBooleans();
    readMinTemps();
    readMaxTemps();
    readBrightness();
    readAddresses();
    Serial.print("eeprom contain :");
    Serial.println(sensorsFromEEPROM->size());
    bool isDataCorrupt = false;
    for (size_t i = 0; i < screen_ls; i++)
    {
        if (maxTemps[i * 2] > 200 || maxTemps[i * 2] < -200 || minTemps[i * 2] > 200 || minTemps[i * 2] < -200 || brightness[0] > 15 || brightness[0] < 0)
        {
            isDataCorrupt = true;
        }
    }

    if (isDataCorrupt)
    {
        Serial.println("corrupted");
        for (size_t i = 0; i < screen_ls; i++)
        {
            maxTemps[i * 2] = 0;
            maxTemps[i * 2 + 1] = 0;
            minTemps[i * 2] = 0;
            minTemps[i * 2 + 1] = 0;
            addresses[i] = 0;
        }
        beeperMuted[0] = true;
        beeperMuted[1] = true;
        beeperInverted[0] = true;
        beeperInverted[1] = true;
        brightness[0] = 7;
        brightness[1] = 7;
        saveMaxTemps();
        saveMinTemps();
        saveBrightness();
        saveBooleans();
    }
}

 bool *Settings::getSensorsInRange()  {
    return sensorsInRange;
}
