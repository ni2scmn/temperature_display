#include <Arduino.h>
#include "button.h"

Button::Button(byte pin_, int mode_, unsigned long debounce_delay_)
{
    pin = pin_;
    last_debounce_time = millis();
    debounce_delay = debounce_delay_;
    last_button_state = HIGH;

    press_response_given = false;
}

bool Button::is_pressed()
{
    int reading = digitalRead(pin);
    if (reading != last_button_state)
    {
        last_debounce_time = millis();
    }

    if ((millis() - last_debounce_time) > debounce_delay)
    {
        if (reading != button_state)
        {
            button_state = reading;
        }
    }
    last_button_state = reading;
    return button_state == LOW;
}

bool Button::is_pressed_new()
{
    if(is_pressed() & !press_response_given) {
        press_response_given = true;
        return true;
    }
    if(is_pressed() & press_response_given) {
        return false;
    }
    else {
        press_response_given = false;
        return false;
    }
}
