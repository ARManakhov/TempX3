#include "Settings.h"

byte * * Settings::getAddressess(){
    return addressess;
}

byte * Settings::getBrightness(){
    return brightness;
}

int * Settings::getMaxTemps(){
    return maxTemps;
}

int * Settings::getMinTemps(){
    return minTemps;
}

bool * Settings::getZoomerInverted(){
    return zoomerInverted;
}

bool * Settings::getZoomerMute(){
    return zoomerMuted;
}

bool Settings::commitAddressess(){
    bool wasTrue = false;
    for(byte i = 0; i<screen_ls; i++){
        if (addressess[i*2] != addressess[i*2 + 1])
        {
            wasTrue = true;
            addressess[i*2] = addressess[i*2 + 1];
            //write to mem
        }
    }
    return wasTrue;
}


bool Settings::commitBrightness(){
    if (brightness[0] != brightness[1])
    {
        brightness[0] = brightness[1];
        //write to mem
        return true;
    }
    return false;
}


bool Settings::commitMaxTemps(){
    bool wasTrue = false;
    for(byte i = 0; i<screen_ls; i++){
        if (maxTemps[i*2] != maxTemps[i*2 + 1])
        {
            wasTrue = true;
            maxTemps[i*2] = maxTemps[i*2 + 1];
            //write to mem
        }
    }
    return wasTrue;
}


bool Settings::commitMinTemps(){
    bool wasTrue = false;
    for(byte i = 0; i<screen_ls; i++){
        if (minTemps[i*2] != minTemps[i*2 + 1])
        {
            wasTrue = true;
            minTemps[i*2] = minTemps[i*2 + 1];
            //write to mem
        }
    }
    return wasTrue;
}


bool Settings::commitZoomerInverted(){
    if (zoomerInverted[0] != zoomerInverted[1])
    {
        zoomerInverted[0] = zoomerInverted[1];
        return true;
    }
    return false;
    
}


bool Settings::commitZoomerMute(){
    if (zoomerMuted[0] != zoomerMuted[1])
    {
        zoomerMuted[0] = zoomerMuted[1];
    }
}

Settings::Settings(){
    //read values to conf from eeprom
    for (size_t i = 0; i < screen_ls; i++)
    {
        maxTemps[i*2] = 0;
        maxTemps[i*2+1] = 0;
        minTemps[i*2]=0;
        minTemps[i*2+1]=0;
        addressess[i]=0;
    }
    zoomerMuted[0]=true;
    zoomerMuted[1]=true;
    zoomerInverted[0]=true;
    zoomerInverted[1]=true;

}