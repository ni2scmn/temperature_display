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
#include "co2_page.h"

const int button_pin = 2, dht_pin = 4;
const int debounce_delay = 50;
const int doubleclick_threshold = 500;
const byte n_pages = 4;

volatile unsigned long last_button_press = 0;

unsigned long last_update = 0, update_interval = 300;
unsigned int display_duration = 5000;
byte selected_page = 0;
volatile bool unhandled_button_click = false, unhandled_double_click = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Display display(&lcd);
DHT dht(dht_pin, DHT22);

TemperaturePage tp(&lcd);
TemperaturePageDHT tpd(&lcd, &dht);
HumidityPage hp(&lcd, &dht);
Co2Page cp(&lcd);

InfoPage *pages[n_pages] = {&cp, &tp, &tpd, &hp};

void button_interrupt()
{
  if ((last_button_press + debounce_delay) > millis())
  {
    return; // assume unwanted bounce
  }

  if (unhandled_button_click & ((last_button_press + doubleclick_threshold) > millis()))
  {
    unhandled_double_click = true;
  }
  else
  {
    unhandled_button_click = true;
  }
  last_button_press = millis();
}

void setup()
{
  Serial.begin(9600);
  lcd.init();
  display.turn_on();
  dht.begin();
  pages[selected_page]->print_page();
  pinMode(button_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), button_interrupt, LOW);
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
  if (unhandled_button_click)
  {
    if (!display.is_on())
    {
      display.turn_on();
      unhandled_button_click = false;
    }
    else if (display.is_on())
    {
      if(unhandled_double_click) {
        pages[selected_page]->reset_page();
        unhandled_double_click = false;
        unhandled_button_click = false;
      }
      else if ((last_button_press + doubleclick_threshold) < millis())
      {
        selected_page = (selected_page + 1) % n_pages;
        pages[selected_page]->print_page();
          unhandled_button_click = false;
      }
    }
  }
  // turn display off again after certain threshold
  if (display.is_on() & (last_button_press + display_duration < millis()))
  {
    display.turn_off();
  }
}