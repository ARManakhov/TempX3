#include "DisplayManager.h"

DisplayManager::DisplayManager(byte *scState, Settings * settings, std::vector<Sensor *> *sensors, Screen* screen) : scState(scState), settings(settings), sensors(sensors), screen(screen)
{
}

DisplayManager::~DisplayManager()
{
}

void DisplayManager::dispatch()
{
    for (size_t i = 0; i < screen_ls; i++)
        {
            switch (scState[i])
            {
            case 0:
                if (sensors->at(i)->isDisconnected())
                {
                    screen->setLineAsErr(i);
                }
                else
                {
                    screen->setLineData(i, sensors->at(i)->getLastTemp());
                }
                break;
            case 1 ... 7:
            {
                screen->setLineAsMenu(i, scState[i]);
                break;
            }
            case 11:{
                screen->setLineData(i,settings->getMinTemps()[i*2+1]);
                break;
            }
            case 12:{
                screen->setLineData(i,settings->getMaxTemps()[i*2+1]);
                break;
            }
            case 13:{
                screen->setLineData(i,settings->getZoomerMute()[1]);
                break;
            }
            case 14:{
                screen->setLineData(i,settings->getZoomerInverted()[1]);
                break;
            }
            case 15:{
                screen->setLineData(i,settings->getBrightness()[1]);
                break;
            }
            default:
                break;
            }
        }
        screen->drawAll();
}
