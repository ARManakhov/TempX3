//
// Created by sirosh on 23.06.2020.
//

#ifndef UNTITLED3_SCREEN_H
#define UNTITLED3_SCREEN_H

#include <Arduino.h>
#include "settingsHW.h"

class Screen
{
public:
    void setLineData(int lineNum, int Num);
    void setLineData(int lineNum, byte* data);
    void setLineAsMenu(int lineNum, int num);
    void setLineAsErr(int lineNum);
    void drawAll();
    Screen();
    void SetTestMode(bool testMode);
    void SetAllOn(bool state);
    void setBrigtness(byte bright);
private:
    byte brightness;
    void init();
    int cyclesCount ;
    byte bytes[screen_ls * screen_cs];
    byte process(int column, int num);
    void drawCol(int num);
    const byte digits[10] = {
        // describe 7 segment displays digits
        // dp a b c d e f g
        B01111110,
        B00110000,
        B01101101,
        B01111001,
        B00110011,
        B01011011,
        B01011111,
        B01110000,
        B01111111,
        B01111011};
    void SendToAllStr(byte code, byte data);
};

#endif //UNTITLED3_SCREEN_H
