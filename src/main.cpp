#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "button.h"
#include "display.h"
#include "utils.h"

const int button_pin = 7;
const int debounce_delay = 50;
const int doubleclick_threshold = 500;

int temperature, max_temperature = -100, min_temperature = 200;
unsigned long last_button_press, last_update = 0, update_interval = 300;
unsigned int display_duration = 5000;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Button button(button_pin, INPUT_PULLUP, debounce_delay);
Display display(&lcd);

void setup()
{
  Serial.begin(9600);
  pinMode(7, INPUT_PULLUP);
  lcd.init();
  display.turn_off();
  lcd.setCursor(1, 0);
  lcd.print("Current/Max/Min:");
}

void loop()
{

  if (check_update_temperature(&last_update, &update_interval))
  {
    temperature = read_temperature();
    update_extreme_temperature(temperature, &min_temperature, &max_temperature);
    print_temperature(&lcd, temperature, min_temperature, max_temperature);
  }

  if (button.is_pressed_new())
  {
      if (!display.is_on())
      {
        display.turn_on();
      }
      else if (display.is_on() & ((last_button_press + doubleclick_threshold) >= millis()))
      {
        reset_extreme_temperature(&min_temperature, &max_temperature);
      }
      last_button_press = millis();
  }

  if (display.is_on() & (last_button_press + display_duration < millis()))
  {
    display.turn_off();
  }
}
