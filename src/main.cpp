#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "button.h"
#include "display.h"

#define new_max(x, y) (((x) >= (y)) ? (x) : (y))
#define new_min(x, y) (((x) <= (y)) ? (x) : (y))

// constants won't change. They're used here to set pin numbers:
const int button_pin = 7; // the number of the pushbutton pin
const int debounce_delay = 50;
const int doubleclick_threshold = 2000;

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

int temperature;
int max_temperature = -100;
int min_temperature = 200;
unsigned long last_update = 0;
unsigned long last_button_press;
unsigned long update_interval = 300;
unsigned int display_duration = 30000;

bool button_was_pressed = false;

void update_extreme_temperature(int temperature, int *min_temperature, int *max_temperature);
bool check_update_temperature(unsigned long *last_update, unsigned long *update_interval);
int read_temperature();
void print_temperature(LiquidCrystal_I2C *lcd, int temperature, int min_temperature, int max_temperature);
void reset_extreme_temperature(int *min_temperature, int *max_temperature);

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
  // Serial.println(button.is_pressed());
  if (button.is_pressed())
  {
    if (button_was_pressed)
    {
      return;
    }
    if (!display.is_on())
    {
      display.turn_on();
    }
    else if (display.is_on() & ((last_button_press + doubleclick_threshold) >= millis()))
    {
      // Serial.println("11");
      reset_extreme_temperature(&min_temperature, &max_temperature);
    }
    last_button_press = millis();
    button_was_pressed = true;
  }
  else {
    button_was_pressed = false;
  }

  if (display.is_on() & (last_button_press + display_duration < millis())) {
    display.turn_off();
  }
}

void update_extreme_temperature(int temperature, int *min_temperature, int *max_temperature)
{
  *max_temperature = new_max(temperature, *max_temperature);
  *min_temperature = new_min(temperature, *min_temperature);
}

bool check_update_temperature(unsigned long *last_update, unsigned long *update_interval)
{
  if (*last_update + *update_interval < millis())
  {
    *last_update = millis();
    return true;
  }
  else
  {
    return false;
  }
}

int read_temperature()
{
  long temp_volt = analogRead(A0);
  int temperature = map(temp_volt, 0, 410, -50, 150);
  return temperature;
}

void print_temperature(LiquidCrystal_I2C *lcd, int temperature, int min_temperature, int max_temperature)
{
  lcd->setCursor(6, 1);
  lcd->print(temperature);
  lcd->setCursor(10, 1);
  lcd->print(max_temperature);
  lcd->setCursor(14, 1);
  lcd->print(min_temperature);
}

void reset_extreme_temperature(int *min_temperature, int *max_temperature)
{
  *min_temperature = 200;
  *max_temperature = -200;
}