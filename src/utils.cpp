#include "utils.h"

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