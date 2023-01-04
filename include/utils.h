#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define new_max(x, y) (((x) >= (y)) ? (x) : (y))
#define new_min(x, y) (((x) <= (y)) ? (x) : (y))

bool check_update_temperature(unsigned long *last_update, unsigned long *update_interval);
void reset_extreme_temperature(int *min_temperature, int *max_temperature);

#endif