#include "InterfaceManager.h"

InterfaceManager::InterfaceManager(byte *scState, Settings settings) : scState(scState), oldSettings(settings)
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
            scState[line] = 7;
            break;
        case 2:
            scState[line] = 1;
            break;
        case 3:
            scState[line] = 2;
            break;
        case 4:
            scState[line] = 3;
            break;
        case 5:
            scState[line] = 4;
            break;
        case 6:
            scState[line] = 5;
            break;
        case 7:
            scState[line] = 6;
            break;
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
        case 3: //menu, option 3 change sensor
            scState[line] = 13;
            break;
        case 4: //menu, option 4 callibrate sensor
            scState[line] = 14;
            break;
        case 5: //menu, option 5 mute buzzer
            scState[line] = 15;
            break;
        case 6: //menu, option 6 buzzer behavior
            scState[line] = 16;
            break;
        case 7: //menu, option 7 screen brightness
            scState[line] = 17;
            break;
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
        case 1:
            scState[line] = 2;
            break;
        case 2:
            scState[line] = 3;
            break;
        case 3:
            scState[line] = 4;
            break;
        case 4:
            scState[line] = 5;
            break;
        case 5:
            scState[line] = 6;
            break;
        case 6:
            scState[line] = 7;
            break;
        case 7:
            scState[line] = 1;
            break;
        default:
            scState[line] = 0;
        }
        break;
        break;
    default:
        break;
    }
}
