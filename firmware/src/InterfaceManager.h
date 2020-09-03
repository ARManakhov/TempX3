#include <Arduino.h>
#include "Settings.h"

class InterfaceManager
{
private:
    byte * scState;
    Settings oldSettings;
    Settings newSettings;
public:
    InterfaceManager(byte * scState, Settings settings);
    ~InterfaceManager();
    void dispatch(byte button, byte i);
};

