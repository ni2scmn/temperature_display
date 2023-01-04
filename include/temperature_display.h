#ifndef TEMPERATURE_DISPLAY_H
#define TEMPERATURE_DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "info_display.h"
#include "utils.h"

class TemperatureDisplay : public InfoDisplay
{
private:
    int temperature;
    int min_temperature;
    int max_temperature;

public:
    void print_page();
    void update_page();
    void reset_page();
    TemperatureDisplay(LiquidCrystal_I2C *lcd_);
};

#endif