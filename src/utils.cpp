#include "utils.h"

bool check_update_page(unsigned long &last_update, unsigned long update_interval)
{
  if (last_update + update_interval < millis())
  {
    last_update = millis();
    return true;
  }
  else
  {
    return false;
  }
}