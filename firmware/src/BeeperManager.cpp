#include "BeeperManager.h"
#include "settingsHW.h"
#include "Settings.h"

void BeeperManager::dispatch() {
    bool canBeep = false;
    if (!settings->getBeeperMute()){
        if (settings->getBeeperInverted()){
            bool * sensorsInRange = settings->getSensorsInRange();
            for (int i = 0; i < screen_ls; ++i) {
                if (!sensorsInRange[i]){
                    canBeep = true;
                    break;
                }
            }
        } else{
            bool * sensorsInRange = settings->getSensorsInRange();
            bool allInRange = true;
            for (int i = 0; i < screen_ls; ++i) {
                if (!sensorsInRange[i]){
                    allInRange = false;
                    break;
                }
            }
            canBeep = allInRange;
        }
    }
    if (canBeep){
        tone(beeper_pin,beeper_frequency); //todo frequently on and off
    }else{
        noTone(beeper_pin);
    }
    delay(100);
}

BeeperManager::BeeperManager(Settings *settings) : settings(settings) {}

