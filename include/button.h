#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button
{
private:
  byte pin;
  byte mode;
  unsigned long debounce_delay;
  unsigned long last_debounce_time;
  int last_button_state;
  int button_state;
  bool press_response_given;

public:
  Button(byte pin_, int mode_, unsigned long debounce_delay_);
  void init();
  bool is_pressed();
  bool is_pressed_new();
};

#endif