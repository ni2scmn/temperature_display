#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define new_max(x, y) (((x) >= (y)) ? (x) : (y))
#define new_min(x, y) (((x) <= (y)) ? (x) : (y))

void update_extreme_temperature(int temperature, int *min_temperature, int *max_temperature);
bool check_update_temperature(unsigned long *last_update, unsigned long *update_interval);
int read_temperature();
void print_temperature(LiquidCrystal_I2C *lcd, int temperature, int min_temperature, int max_temperature);
void reset_extreme_temperature(int *min_temperature, int *max_temperature);


#endif