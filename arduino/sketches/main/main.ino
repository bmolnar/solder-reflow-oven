#include "ReflowOven.h"

ReflowOven oven(Serial, 10, 9, 8, 7);

void setup(void)
{
  Serial.begin(38400);
}
void loop(void)
{
  oven.run();
}


