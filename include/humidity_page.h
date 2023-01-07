#ifndef HUMIDITY_PAGE_H
#define HUMIDITY_PAGE_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#include "info_page.h"
#include "utils.h"

class HumidityPage : public InfoPage
{
private:
    DHT *dht;
    float humidity;
    float min_humidity;
    float max_humidity;

public:
    void print_page();
    void update_page();
    void reset_page();
    void update_measurement();
    float return_measurement();
    HumidityPage(LiquidCrystal_I2C *lcd_, DHT *dht_);
};

#endif