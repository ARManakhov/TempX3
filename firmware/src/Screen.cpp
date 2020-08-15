//
// Created by sirosh on 23.06.2020.
//

#include "Screen.h"
#include <Arduino.h>
#include "settings.h"
#include "max7221codes.h"
#include <SPI.h>

void Screen::setLineData(int lineNum, int num)
{
    if (lineNum <= screen_ls)
    {
        for (int i = screen_cs - 1; i >= 0; --i)
        {
            int digit = num % 10;
            num /= 10;
            bytes[lineNum * screen_cs + i] = process(i, digit);
        }
    }
}

void Screen::setLineData(int lineNum, byte *data)
{
    if (lineNum <= screen_ls)
    {
        for (int i = screen_cs - 1; i >= 0; --i)
        {
            bytes[lineNum * screen_cs + i] = data[i];
        }
    }
}

void Screen::drawAll()
{
    for (int i = 0; i < screen_ls; ++i)
    {
        digitalWrite(cs_pin, LOW);

        drawCol(i);

        digitalWrite(cs_pin, HIGH);
    }
    digitalWrite(cs_pin, LOW);

    if (cyclesCount++ == 500) //reinit periodicaly due to unknown bug
    {
        init();
        cyclesCount = 0;
        Serial.println("reinit screen");
    }
    digitalWrite(cs_pin, HIGH);
}

void Screen::drawCol(int num)
{
    for (int j = 0; j < screen_cs; ++j)
    {
        SPI.transfer(num + 1);
        SPI.transfer(bytes[j * screen_ls + num]);
    }
}

byte Screen::process(int column, int digit)
{
    if (column <= screen_cs && digit <= 10)
    {

        bool b[8];

        for (int i = 0; i < 8; ++i)
        {
            b[i] = (digits[digit] & (1 << i)) != 0;
        }

        int recivedID = 0;

        for (int i = 0; i < 8; i++)
        {
            recivedID |= b[i] << i;
        }

        return recivedID;
    }
    return 0;
}

Screen::Screen()
{
    cyclesCount = 0;

    SPI.begin();
    digitalWrite(cs_pin, LOW);
    init();
    digitalWrite(cs_pin, HIGH);

    byte data[3] = {B00000001, B00000001, B00000001};

    for (int i = 0; i < screen_ls; ++i)
    {
        setLineData(i, data);
    }

    drawAll();
}

void Screen::init()
{
    SendToAllStr(OP_DISPLAYTEST, 0); //quit test mode

    SendToAllStr(OP_SCANLIMIT, screen_cs); // We need the multiplexer to scan all segments

    SendToAllStr(OP_DECODEMODE, 0); // We don't want the multiplexer to decode segments for us

    SendToAllStr(OP_SHUTDOWN, 1); //turn on displays

    SendToAllStr(OP_INTENSITY, 7); //set intensity
}

void Screen::SendToAllStr(byte code, byte data)
{
    for (int i = 0; i < screen_cs; ++i)
    {
        digitalWrite(cs_pin, LOW);
        SPI.transfer(code);
        SPI.transfer(data);
        digitalWrite(cs_pin, HIGH);
    }
}

void Screen::SetTestMode(bool testMode)
{                                                   //only for test
    SendToAllStr(OP_DISPLAYTEST, testMode ? 1 : 0); //quit test mode
}

void Screen::SetAllOn(bool state)
{ //only for test
    for (int j = 1; j <= 8; j++)
    {
        digitalWrite(cs_pin, LOW);
        for (int i = 0; i < 3; ++i)
        {
            SPI.transfer(j);
            SPI.transfer(state ? B11111111 : B00000000);
        }
        digitalWrite(cs_pin, HIGH);
    }
}
