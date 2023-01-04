#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

class Display
{
private:
    LiquidCrystal_I2C *_lcd;
    bool _state;
    unsigned long _last_change;

public:
    Display(LiquidCrystal_I2C *lcd);
    bool is_on();
    unsigned long last_change();
    void turn_on();
    void turn_off();
};

#endif