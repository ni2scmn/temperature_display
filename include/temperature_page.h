#ifndef TEMPERATURE_PAGE_H
#define TEMPERATURE_PAGE_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "info_page.h"
#include "utils.h"

class TemperaturePage : public InfoPage
{
private:
    int temperature;
    int min_temperature;
    int max_temperature;

public:
    void print_page();
    void update_page();
    void reset_page();
    void update_measurement();
    TemperaturePage(LiquidCrystal_I2C *lcd_);
};

#endif