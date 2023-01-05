#include "co2_page.h"

void Co2Page::print_page()
{
    lcd->clear();
    lcd->setCursor(5, 0);
    lcd->print("CO2");
    lcd->setCursor(0, 1);
    lcd->print("N:");
    // lcd->setCursor(5,1);
    // lcd->print("H:");
    // lcd->setCursor(10,1);
    // lcd->print("L:");
    // update_page();
}

void Co2Page::update_measurement() {
    int ppmrange = 5000;
    unsigned long pwmtime = pulseIn(5, HIGH, 2000000) / 1000;
    float pulsepercent = pwmtime / 1004.0;
    co2_ppm = ppmrange * pulsepercent;

    max_co2_ppm = new_max(co2_ppm, max_co2_ppm);
    min_co2_ppm = new_min(co2_ppm, min_co2_ppm);
}

void Co2Page::update_page()
{
    lcd->setCursor(2, 1);
    lcd->print("      ");
    lcd->setCursor(2, 1);
    lcd->print(co2_ppm);
    // lcd->setCursor(7, 1);
    // lcd->print(max_temperature);
    // lcd->setCursor(12, 1);
    // lcd->print(min_temperature);
}

Co2Page::Co2Page(LiquidCrystal_I2C *lcd_) : InfoPage(lcd_)
{
    pinMode(5, INPUT);
    co2_ppm = -100;
    max_co2_ppm = -100;
    min_co2_ppm = 20000;
}

void Co2Page::reset_page()
{
    min_co2_ppm = 20000;
    max_co2_ppm = -100;
}