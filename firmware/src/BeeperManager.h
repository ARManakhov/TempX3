#ifndef BEEPERMANAGER_H
#define BEEPERMANAGER_H


#include "Settings.h"

class BeeperManager {
private:
    Settings * settings;
    void beep();
public:
    void dispatch();
    BeeperManager(Settings *settings);
};


#endif