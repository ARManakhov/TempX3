#include <Arduino.h>
#include <HardwareSerial.h>
#include "settings.h"
#include "Screen.h"
#include "vector"
#include "Sensor.h"
#include "SensorUtil.h"
#include <Arduino_FreeRTOS.h>

using namespace std;

vector<Sensor *> *sensors;
Screen *screen;
SensorUtil sensorUtil;
byte scState[screen_ls];

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
        for (size_t i = 0; i < sensors->size(); i++)
        {
            switch (scState[i])
            {
            case 0:
                if (sensors->at(i)->isDisconnected())
                {
                    byte data[3] = {B00000001, B00000001, B00000001};
                    screen->setLineData(i, data);
                }
                else
                {
                    screen->setLineData(i, sensors->at(i)->getLastTemp());
                }
                break;
            case 1:
            {
                byte data[3] = {B01001110, B00110000, B01111000};
                screen->setLineData(i, data);
                break;
            }

            case 2:
            {
                byte data[3] = {B01001110, B01101101, B01111000};
                screen->setLineData(i, data);
                break;
            }
            case 3:
            {
                byte data[3] = {B01001110, B01111001, B01111000};
                screen->setLineData(i, data);
                break;
            }
            case 4:
            {
                byte data[3] = {B01001110, B00110011, B01111000};
                screen->setLineData(i, data);
                break;
            }
            case 5:
            {
                byte data[3] = {B01001110, B01011011, B01111000};
                screen->setLineData(i, data);
                break;
            }
            case 6:
            {
                byte data[3] = {B01001110, B01011111, B01111000};
                screen->setLineData(i, data);
                break;
            }
            case 7:
            {
                byte data[3] = {B01001110, B01110000, B01111000};
                screen->setLineData(i, data);
                break;
            }
            default:
                break;
            }
        }
        screen->drawAll();
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
            switch (butNum)
            {
            case 0:
                switch (scState[i])
                {
               case 0:
                    scState[i] = 1;
                    break;
                case 1:
                    scState[i] = 7;
                    break;
                case 2:
                    scState[i] = 1;
                    break;
                case 3:
                    scState[i] = 2;
                    break;
                case 4:
                    scState[i] = 3;
                    break;
                case 5:
                    scState[i] = 4;
                    break;
                case 6:
                    scState[i] = 5;
                    break;
                case 7:
                    scState[i] = 6;
                    break;
                default:
                    scState[i] = 0;
                }
                break;
            case 1:
                switch (scState[i])
                {
                case 0: //just view temp
                    scState[i] = 1;
                    break;
                case 1: //menu, option 1 min trigger temp
                    scState[i] = 11;
                    break;
                case 2: //menu, option 2 max treigger temp
                    scState[i] = 12;
                    break;
                case 3: //menu, option 3 change sensor
                    scState[i] = 13;
                    break;
                case 4: //menu, option 4 callibrate sensor
                    scState[i] = 14;
                    break;
                case 5: //menu, option 5 mute buzzer
                    scState[i] = 15;
                    break;
                case 6: //menu, option 6 buzzer behavior
                    scState[i] = 16;
                    break;
                case 7: //menu, option 7 screen brightness
                    scState[i] = 17;
                    break;
                default:
                    scState[i] = 0;
                }
                break;
            case 2:
                switch (scState[i])
                {
                case 0:
                    scState[i] = 2;
                    break;
                case 1:
                    scState[i] = 2;
                    break;
                case 2:
                    scState[i] = 3;
                    break;
                case 3:
                    scState[i] = 4;
                    break;
                case 4:
                    scState[i] = 5;
                    break;
                case 5:
                    scState[i] = 6;
                    break;
                case 6:
                    scState[i] = 7;
                    break;
                case 7:
                    scState[i] = 1;
                    break;
                default:
                    scState[i] = 0;
                }
                break;
                break;
            default:
                break;
            }

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

    xTaskCreate(TaskSensorsRead, "SensorRead", 128, NULL, 0, NULL);
    xTaskCreate(TaskScreenDraw, "ScreenDraw", 128, NULL, 0, NULL);
    xTaskCreate(TaskButtonRead, "ButtonRead", 128, NULL, 0, NULL);
}

void loop() {}
