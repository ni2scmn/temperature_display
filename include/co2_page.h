#ifndef CO2_PAGE_H
#define CO2_PAGE_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#include "info_page.h"
#include "utils.h"

class Co2Page : public InfoPage
{
private:
    int co2_ppm;
    int max_co2_ppm;
    int min_co2_ppm;

public:
    void print_page();
    void update_page();
    void reset_page();
    void update_measurement();
    int return_measurement();
    Co2Page(LiquidCrystal_I2C *lcd_);
};

#endif