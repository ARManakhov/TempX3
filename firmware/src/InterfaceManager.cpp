#include "InterfaceManager.h"

InterfaceManager::InterfaceManager(byte *scState, Settings *settings) : scState(scState), settings(settings)
{
}

InterfaceManager::~InterfaceManager()
{
}

void InterfaceManager::dispatch(byte button, byte line)
{
    switch (button)
    {
    case 0:
        switch (scState[line])
        {
        case 0:
            scState[line] = 1;
            break;
        case 1:
            scState[line] = 5;
            break;
        case 2 ... 5:
            scState[line]--;
            break;
        case 11:
        {
            if (settings->getMinTemps()[line * 2 + 1] == -200)
            {
                settings->getMinTemps()[line * 2 + 1] = 200;
            }
            else
            {
                --settings->getMinTemps()[line * 2 + 1];
            }
            break;
        }
        case 12:
        {
            if (settings->getMaxTemps()[line * 2 + 1] == 200)
            {
                settings->getMaxTemps()[line * 2 + 1] = 200;
            }
            else
            {   
                --settings->getMaxTemps()[line * 2 + 1];
            }
            break;
        }
        case 13:
        {
            settings->getZoomerMute()[1] = !settings->getZoomerMute()[1];
            break;
        }
        case 14:
        {
            settings->getZoomerInverted()[1] = !settings->getZoomerInverted()[1];
            break;
        }
        case 15:
        {
            if (settings->getBrightness()[1] != 0)
            {
                --settings->getBrightness()[1];
            }
            else
            {
                settings->getBrightness()[1] = 15;
            }
            break;
        }
        default:
            scState[line] = 0;
        }
        break;
    case 1:
        switch (scState[line])
        {
        case 0: //just view temp
            scState[line] = 1;
            break;
        case 1: //menu, option 1 min trigger temp
            scState[line] = 11;
            break;
        case 2: //menu, option 2 max treigger temp
            scState[line] = 12;
            break;
        case 3: //menu, option 3 mute buzzer
            scState[line] = 13;
            break;
        case 4: //menu, option 4 buzzer behavior
            scState[line] = 14;
            break;
        case 5: //menu, option 5 screen brightness
            scState[line] = 15;
            break;
        case 6: //menu, option 6 change sensor
            scState[line] = 16;
            break;
        case 7: //menu, option 7 callibrate sensor
            scState[line] = 17;
            break;
        case 11:
        {
            settings->commitMinTemps();
            scState[line] = 0;
            break;
        }
        case 12:
        {
            settings->commitMaxTemps();
            scState[line] = 0;
            break;
        }
        case 13:
        {
            settings->commitZoomerMute();
            scState[line] = 0;
            break;
        }
        case 14:
        {
            settings->commitZoomerInverted();
            scState[line] = 0;
            break;
        }
        case 15:
        {
            settings->commitBrightness();
            scState[line] = 0;
            break;
        }
        default:
            scState[line] = 0;
        }
        break;
    case 2:
        switch (scState[line])
        {
        case 0:
            scState[line] = 2;
            break;
        case 1 ... 4:
            scState[line]++;
            break;
        case 5:
            scState[line] = 1;
            break;
        case 11:
        {
            if (settings->getMinTemps()[line * 2 + 1] == 200)
            {
                settings->getMinTemps()[line * 2 + 1] = -200;
            }
            else
            {
                ++settings->getMinTemps()[line * 2 + 1];
            }
            break;
        }
        case 12:
        {
            if (settings->getMaxTemps()[line * 2 + 1] == 200)
            {
                settings->getMaxTemps()[line * 2 + 1] = -200;
            }
            else
            {
                ++settings->getMaxTemps()[line * 2 + 1];
            }
            break;
        }
        case 13:
        {
            settings->getZoomerMute()[1] = !settings->getZoomerMute()[1];
            break;
        }
        case 14:
        {
            settings->getZoomerInverted()[1] = !settings->getZoomerInverted()[1];
            break;
        }
        case 15:
        {
            if (settings->getBrightness()[1] != 15)
            {
                ++settings->getBrightness()[1];
            }
            else
            {
                settings->getBrightness()[1] = 0;
            }
            break;
        }
        default:
            scState[line] = 0;
        }
        break;
        break;
    default:
        break;
    }
}
