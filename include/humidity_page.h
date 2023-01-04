#ifndef HUMIDITY_PAGE_H
#define HUMIDITY_PAGE_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "info_page.h"
#include "utils.h"

class HumidityPage : public InfoPage
{
private:
    int humidity;
    int min_humidity;
    int max_humidity;

public:
    void print_page();
    void update_page();
    void reset_page();
    void update_measurement();
    HumidityPage(LiquidCrystal_I2C *lcd_);
};

#endif