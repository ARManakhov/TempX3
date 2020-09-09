#ifndef INTERFACE_MANAGER_H
#define INTERFACE_MANAGER_H

#include <Arduino.h>
#include "Settings.h"

class InterfaceManager
{
private:
    byte * scState;
    Settings * settings;
public:
    InterfaceManager(byte * scState, Settings * settings);
    ~InterfaceManager();
    void dispatch(byte button, byte i);
};
#endif