#include "temperature_display.h"

void TemperatureDisplay::print_page()
{
    lcd->setCursor(1, 0);
    lcd->print("Current/Max/Min:");
    update_page();
}

void TemperatureDisplay::update_page()
{
    long temp_volt = analogRead(A0);
    temperature = map(temp_volt, 0, 410, -50, 150);

    max_temperature = new_max(temperature, max_temperature);
    min_temperature = new_min(temperature, min_temperature);

    lcd->setCursor(6, 1);
    lcd->print(temperature);
    lcd->setCursor(10, 1);
    lcd->print(max_temperature);
    lcd->setCursor(14, 1);
    lcd->print(min_temperature);
}

TemperatureDisplay::TemperatureDisplay(LiquidCrystal_I2C *lcd_) : InfoDisplay(lcd_)
{
    temperature = -100;
    max_temperature = -100;
    min_temperature = 200;
}

void TemperatureDisplay::reset_page()
{
    min_temperature = 200;
    max_temperature = -100;
}