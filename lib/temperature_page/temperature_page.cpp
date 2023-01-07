#include "temperature_page.h"

void TemperaturePage::print_page()
{
    lcd->clear();
    lcd->setCursor(4, 0);
    lcd->print("Temperature");
    lcd->setCursor(0, 1);
    lcd->print("N:");
    lcd->setCursor(5, 1);
    lcd->print("H:");
    lcd->setCursor(10, 1);
    lcd->print("L:");
    update_page();
}

void TemperaturePage::update_measurement()
{
    temperature = dht->readTemperature();
    max_temperature = new_max(temperature, max_temperature);
    min_temperature = new_min(temperature, min_temperature);
}

float TemperaturePage::return_measurement()
{
    return temperature;
}

void TemperaturePage::update_page()
{
    lcd->setCursor(2, 1);
    lcd->print((int)round(temperature));
    lcd->setCursor(7, 1);
    lcd->print((int)round(max_temperature));
    lcd->setCursor(12, 1);
    lcd->print((int)round(min_temperature));
}

TemperaturePage::TemperaturePage(LiquidCrystal_I2C *lcd_, DHT *dht_) : InfoPage(lcd_)
{
    dht = dht_;
    temperature = -100;
    max_temperature = -100;
    min_temperature = 200;
}

void TemperaturePage::reset_page()
{
    min_temperature = 200;
    max_temperature = -100;
}