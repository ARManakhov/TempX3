#include "Settings.h"
#include <EEPROM.h>
#include "Screen.h"

byte **Settings::getAddressess()
{
    return addressess;
}

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

bool *Settings::getZoomerInverted()
{
    return zoomerInverted;
}

bool *Settings::getZoomerMute()
{
    return zoomerMuted;
}

bool Settings::commitAddressess()
{
    bool wasTrue = false;
    for (byte i = 0; i < screen_ls; i++)
    {
        if (addressess[i * 2] != addressess[i * 2 + 1])
        {
            wasTrue = true;
            addressess[i * 2] = addressess[i * 2 + 1];
            saveBools();
        }
    }
    return wasTrue;
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

bool Settings::commitZoomerInverted()
{
    if (zoomerInverted[0] != zoomerInverted[1])
    {
        zoomerInverted[0] = zoomerInverted[1];
        saveBools();
        return true;
    }
    return false;
}

bool Settings::commitZoomerMute()
{
    if (zoomerMuted[0] != zoomerMuted[1])
    {
        zoomerMuted[0] = zoomerMuted[1];
    }
    return true;
}

bool Settings::saveBools()
{
    byte bools = (zoomerMuted[0] << 1) + zoomerInverted[0];
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

bool Settings::readBools()
{
    byte bools = EEPROM.read(1);
    zoomerInverted[0] = bools & 0xFE;
    zoomerInverted[1] = zoomerInverted[0];
    zoomerMuted[0] = bools >> 1;
    zoomerMuted[1] = zoomerMuted[0];
    return true;
}

bool Settings::readMaxTemps()
{
    for (size_t i = 0; i < screen_ls; i++)
    {
        maxTemps[i * 2] = (EEPROM.read(3 + screen_ls * 2 + i * 2 + 0) << 8 )+ EEPROM.read(3 + screen_ls * 2 + i * 2 + 1);
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

bool Settings::readBrightness(){
    brightness[0] = EEPROM.read(2);
    brightness[1] = brightness[0];
    screen->setBrigtness(brightness[0]);
    return true;
}

Settings::Settings(Screen * screen) : screen(screen)
{
    //read values to conf from eeprom
    readBools();
    readMinTemps();
    readMaxTemps();
    readBrightness();
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
            addressess[i] = 0;
        }
        zoomerMuted[0] = true;
        zoomerMuted[1] = true;
        zoomerInverted[0] = true;
        zoomerInverted[1] = true;
        brightness[0] = 7;
        brightness[1] = 7;
        saveMaxTemps();
        saveMinTemps();
        saveBrightness();
        saveBools();
    }
}
