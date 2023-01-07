#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>
#include <SD.h>
#include <SPI.h>
#include <RTClib.h>

#include "button.h"
#include "display.h"
#include "utils.h"
#include "temperature_page.h"
#include "humidity_page.h"
#include "co2_page.h"

void log_values(RTC_DS1307 *rtc, int measurements[], int n_pages);

const int button_pin = 2, dht_pin = 4;
const int debounce_delay = 50;
const int doubleclick_threshold = 500;

volatile unsigned long last_button_press = 0;

unsigned long last_update = 0, last_log = 0, page_update_interval = 300, log_interval = 2000;
unsigned int display_duration = 5000;
byte selected_page = 0;
volatile bool unhandled_button_click = false, unhandled_double_click = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Display display(&lcd);
DHT dht(dht_pin, DHT22);
RTC_DS1307 rtc;
File log_file;

TemperaturePage tp(&lcd, &dht);
HumidityPage hp(&lcd, &dht);
Co2Page cp(&lcd);

const byte n_pages = 3;
InfoPage *pages[n_pages] = {&cp, &tp, &hp};
int *measurements = new int[n_pages];
const String LOG_HEADER = "time, co2, temperature, humidity,";

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

#ifndef ESP8266
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB
#endif

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1)
      delay(10);
  }

  if (!rtc.isrunning())
  {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (!SD.begin(10))
  { // TODO pin as config
    Serial.println("SD card initialization failed...");
  }
  else
  {
    Serial.println("SD card initialized");

    if (!SD.exists("tests.txt"))
    {
      Serial.println("file does not exist");
      log_file = SD.open("tests.txt", FILE_WRITE);
      log_file.println(LOG_HEADER);
      log_file.close();
    }
    else
    {
      Serial.println("file exists");
    }
  }

  lcd.init();
  display.turn_on();
  dht.begin();
  pages[selected_page]->print_page();
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), button_interrupt, LOW);
}

void loop()
{
  // update page if neccessary
  if (check_update(last_update, page_update_interval))
  {

    for (size_t i = 0; i < n_pages; i++)
    {
      pages[i]->update_measurement();
      measurements[i] = pages[i]->return_measurement();
    }

    pages[selected_page]->update_page();
  }

  if (check_update(last_log, log_interval))
  {
    log_values(&rtc, measurements, n_pages);
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
      if (unhandled_double_click)
      {
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

void log_values(RTC_DS1307 *rtc, int measurements[], int n_pages)
{
  log_file = SD.open("tests.txt", FILE_WRITE);

  if (!log_file)
  {
    Serial.println("ELT");
  }

  DateTime now = rtc->now();

  log_file.print(now.year(), DEC);
  log_file.print('/');
  log_file.print(now.month(), DEC);
  log_file.print('/');
  log_file.print(now.day(), DEC);
  log_file.print(" ");
  log_file.print(now.hour(), DEC);
  log_file.print(':');
  log_file.print(now.minute(), DEC);
  log_file.print(':');
  log_file.print(now.second(), DEC);
  log_file.print(",");

  for (size_t i = 0; i < n_pages; i++)
  {
    log_file.print(measurements[i]);
    log_file.print(",");
  }

  log_file.println();
  log_file.flush();

  log_file.close();
}