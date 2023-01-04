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

const int button_pin = 8, dht_pin = 4;
const int debounce_delay = 50;
const int doubleclick_threshold = 500;
const byte n_pages = 4;

unsigned long last_button_press, last_update = 0, update_interval = 300;
unsigned int display_duration = 15000;
byte selected_page = 0;
bool page_switch_requested = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Button button(button_pin, INPUT_PULLUP, debounce_delay);
Display display(&lcd);
DHT dht(dht_pin, DHT22);

TemperaturePage tp(&lcd);
TemperaturePageDHT tpd(&lcd, &dht);
HumidityPage hp(&lcd, &dht);
Co2Page cp(&lcd);

InfoPage *pages[n_pages] = {&cp, &tp, &tpd, &hp};

void setup()
{
  Serial.begin(9600);
  button.init();
  lcd.init();
  display.turn_on();
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
    // TODO disabled;
    // display.turn_off();
  }
}



// #include <Arduino.h>
// #include <LiquidCrystal_I2C.h>
// #include <Wire.h>
// #include "DHT.h"

// int DataPin = 5;
// int ppmrange = 5000;
// unsigned long pwmtime;

// int PPM = 0;
// float pulsepercent=0;


// void setup() {
//  pinMode(DataPin, INPUT);
//  Serial.begin(9600);
// }

// void loop() {  
//  pwmtime = pulseIn(DataPin, HIGH, 2000000) / 1000;
//  float pulsepercent = pwmtime / 1004.0;
//  PPM = ppmrange * pulsepercent;
//  Serial.print("CO2 Konzentration in der Luft in PPM: ");
//  Serial.println(PPM);
//  delay(5000);
// }