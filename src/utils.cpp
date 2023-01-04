#include "utils.h"

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

void reset_extreme_temperature(int *min_temperature, int *max_temperature)
{
  *min_temperature = 200;
  *max_temperature = -200;
}