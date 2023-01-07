#ifndef INFO_PAGE_H
#define INFO_PAGE_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
class InfoPage
{
protected:
    LiquidCrystal_I2C *lcd;

public:
    virtual void print_page() = 0;
    virtual void update_page() = 0;
    virtual void reset_page() = 0;
    virtual void update_measurement() = 0;
    virtual int return_measurement() = 0;
    InfoPage(LiquidCrystal_I2C *lcd_);
};

#endif