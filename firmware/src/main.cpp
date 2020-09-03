#include <Arduino.h>
#include <HardwareSerial.h>
#include "settingsHW.h"
#include "Screen.h"
#include "vector"
#include "Sensor.h"
#include "SensorUtil.h"
#include <Arduino_FreeRTOS.h>
#include "InterfaceManager.h"
#include "DisplayManager.h"

using namespace std;

vector<Sensor *> *sensors;
Screen *screen;
SensorUtil sensorUtil;
byte scState[screen_ls];
Settings settings;
InterfaceManager *interfaceManager;
DisplayManager *displayManager;

void TaskSensorsRead(void *pvParameters)
{
    for (;;)
    {
        for (size_t i = 0; i < sensors->size(); i++)
        {
            sensors->at(i)->readData();
            sensors->at(i)->getTemp();
        }
    }
}

void TaskScreenDraw(void *pvParameters)
{
    for (;;)
    {
        displayManager->dispatch();
        delay(50);
    }
}

void TaskButtonRead(void *pvParameters)
{
    const byte buttonPins[screen_ls] = button_pins;
    const int buttonCurents[3] = button_currents;
    unsigned long butTime[screen_ls * 3];
    for (size_t i = 0; i < screen_ls * 3; i++)
    {
        butTime[i] = 0;
    }

    for (;;)
    {
        for (size_t i = 0; i < screen_ls; i++)
        {
            int butNum = -1;
            int aRead = analogRead(buttonPins[i]);
            for (size_t j = 0; j < 3; j++)
            {
                if (aRead + 50 > buttonCurents[j] && aRead - 50 < buttonCurents[j])
                {
                    if (butTime[i * 3 + j] + 100 < millis())
                    {
                        butNum = j;
                        butTime[i * screen_ls + j] = millis();
                    }
                    break;
                }
            }
            interfaceManager->dispatch(butNum, i);         
            delay(20);
            //Serial.println(butNum);
        }
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("starting device");
    pinMode(cs_pin, OUTPUT);
    sensors = new vector<Sensor *>();
    screen = new Screen();
    sensorUtil.scanAllNew(sensors, OneWire(sensors_pin));

    for (size_t i = 0; i < screen_ls; i++)
    {
        scState[i] = 0;
    }

    interfaceManager = new InterfaceManager(scState, settings);
    displayManager = new DisplayManager(scState, settings, sensors, screen);

    xTaskCreate(TaskSensorsRead, "SensorRead", 128, NULL, 0, NULL);
    xTaskCreate(TaskScreenDraw, "ScreenDraw", 128, NULL, 0, NULL);
    xTaskCreate(TaskButtonRead, "ButtonRead", 128, NULL, 0, NULL);
}

void loop() {}
