#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "DHT.h"

#include "button.h"
#include "display.h"
#include "utils.h"
#include "temperature_page.h"
#include "temperature_page_dht.h"
#include "humidity_page.h"

const int button_pin = 7, dht_pin = 4;
const int debounce_delay = 50;
const int doubleclick_threshold = 500;
const byte n_pages = 3;

unsigned long last_button_press, last_update = 0, update_interval = 300;
unsigned int display_duration = 5000;
byte selected_page = 0;
bool page_switch_requested = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Button button(button_pin, INPUT_PULLUP, debounce_delay);
Display display(&lcd);
DHT dht(dht_pin, DHT22);

TemperaturePage tp(&lcd);
TemperaturePageDHT tpd(&lcd, &dht);
HumidityPage hp(&lcd, &dht);
InfoPage *pages[n_pages] = {&tp, &tpd, &hp};

void setup()
{
  Serial.begin(9600);
  button.init();
  lcd.init();
  display.turn_off();
  dht.begin();
  pages[selected_page]->print_page();
}

void loop()
{

  // update page if neccessary
  if (check_update_page(last_update, update_interval))
  {
    for (auto p : pages)
    {
      p->update_measurement();
    }
    pages[selected_page]->update_page();
  }

  // handle button click
  if (button.is_pressed_new())
  {
    if (!display.is_on())
    {
      display.turn_on();
    }
    else if (display.is_on())
    {
      if ((last_button_press + doubleclick_threshold) >= millis())
      {
        pages[selected_page]->reset_page();
        page_switch_requested = false;
      }
      else
      {
        page_switch_requested = true;
      }
    }
    last_button_press = millis();
  }

  if (page_switch_requested & ((last_button_press + doubleclick_threshold) < millis()))
  {
    selected_page = (selected_page + 1) % n_pages;
    pages[selected_page]->print_page();
    page_switch_requested = false;
  }

  // turn display off again after certain threshold
  if (display.is_on() & (last_button_press + display_duration < millis()))
  {
    display.turn_off();
  }
}
