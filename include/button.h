#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button
{
private:
  byte pin;
  unsigned long debounce_delay;
  unsigned long last_debounce_time;
  int last_button_state;
  int button_state;

public:
  Button(byte pin, int mode, unsigned long debounce_delay);
  bool is_pressed();
};

#endif