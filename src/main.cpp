#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "button.h"
#include "display.h"
#include "utils.h"
#include "temperature_page.h"

const int button_pin = 7;
const int debounce_delay = 50;
const int doubleclick_threshold = 500;

unsigned long last_button_press, last_update = 0, update_interval = 300;
unsigned int display_duration = 5000;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Button button(button_pin, INPUT_PULLUP, debounce_delay);
Display display(&lcd);

TemperaturePage tp(&lcd);
InfoPage *pages[1] = {&tp};

void setup()
{
  Serial.begin(9600);
  pinMode(7, INPUT_PULLUP);
  lcd.init();
  display.turn_off();
  pages[0]->print_page();
}

void loop()
{

  if (check_update_temperature(&last_update, &update_interval))
  {
    pages[0]->update_page();
  }

  if (button.is_pressed_new())
  {
    if (!display.is_on())
    {
      display.turn_on();
    }
    else if (display.is_on() & ((last_button_press + doubleclick_threshold) >= millis()))
    {
      pages[0]->reset_page();
    }
    last_button_press = millis();
  }

  if (display.is_on() & (last_button_press + display_duration < millis()))
  {
    display.turn_off();
  }
}
