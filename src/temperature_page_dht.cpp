#include "temperature_page_dht.h"

void TemperaturePageDHT::print_page()
{
    lcd->clear();
    lcd->setCursor(4, 0);
    lcd->print("TemperatureD");
    lcd->setCursor(0, 1);
    lcd->print("N:");
    lcd->setCursor(5,1);
    lcd->print("H:");
    lcd->setCursor(10,1);
    lcd->print("L:");
    update_page();
}

void TemperaturePageDHT::update_measurement() {
    temperature = (int)round(dht->readTemperature());
    max_temperature = new_max(temperature, max_temperature);
    min_temperature = new_min(temperature, min_temperature);
}

void TemperaturePageDHT::update_page()
{
    lcd->setCursor(2, 1);
    lcd->print(temperature);
    lcd->setCursor(7, 1);
    lcd->print(max_temperature);
    lcd->setCursor(12, 1);
    lcd->print(min_temperature);
}

TemperaturePageDHT::TemperaturePageDHT(LiquidCrystal_I2C *lcd_, DHT *dht_) : InfoPage(lcd_)
{
    dht = dht_;
    temperature = -100;
    max_temperature = -100;
    min_temperature = 200;
}

void TemperaturePageDHT::reset_page()
{
    min_temperature = 200;
    max_temperature = -100;
}