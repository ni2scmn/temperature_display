#include "display.h"

Display::Display(LiquidCrystal_I2C *lcd)
{
    _lcd = lcd;
}

bool Display::is_on()
{
    return _state;
}

unsigned long Display::last_change()
{
    return _last_change;
}

void Display::turn_off()
{
    _lcd->noDisplay();
    _lcd->noBacklight();
    _state = false;
    _last_change = millis();
}

void Display::turn_on()
{
    _lcd->display();
    _lcd->backlight();
    _state = true;
    _last_change = millis();
}