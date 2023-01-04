#include "humidity_page.h"

void HumidityPage::print_page()
{
    lcd->clear();
    lcd->setCursor(8, 0);
    lcd->print("Humidity");
    lcd->setCursor(0, 1);
    lcd->print("N:");
    lcd->setCursor(5,1);
    lcd->print("H:");
    lcd->setCursor(10,1);
    lcd->print("L:");
    update_page();
}

void HumidityPage::update_page()
{
    long temp_volt = analogRead(A0);
    humidity = map(temp_volt, 0, 410, -50, 150);

    max_humidity = new_max(humidity, max_humidity);
    min_humidity = new_min(humidity, min_humidity);

    lcd->setCursor(2, 1);
    lcd->print(humidity);
    lcd->setCursor(7, 1);
    lcd->print(max_humidity);
    lcd->setCursor(12, 1);
    lcd->print(min_humidity);
}

HumidityPage::HumidityPage(LiquidCrystal_I2C *lcd_) : InfoPage(lcd_)
{
    humidity = -100;
    max_humidity = -100;
    min_humidity = 200;
}

void HumidityPage::reset_page()
{
    min_humidity = 200;
    max_humidity = -100;
}