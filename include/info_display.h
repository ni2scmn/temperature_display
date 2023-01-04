#ifndef INFO_DISPLAY_H
#define INFO_DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
class InfoDisplay
{
protected:
    LiquidCrystal_I2C *lcd;

public:
    virtual void print_page() = 0;
    virtual void update_page() = 0;
    virtual void reset_page() = 0;
    InfoDisplay(LiquidCrystal_I2C *lcd_);
};

#endif