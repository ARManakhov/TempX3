#include "DisplayManager.h"

DisplayManager::DisplayManager(byte *scState, Settings settings, std::vector<Sensor *> *sensors, Screen* screen) : scState(scState), oldSettings(settings), sensors(sensors), screen(screen)
{
}

DisplayManager::~DisplayManager()
{
}

void DisplayManager::dispatch()
{
    for (size_t i = 0; i < sensors->size(); i++)
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
            default:
                break;
            }
        }
        screen->drawAll();
}
