#include <Arduino.h>

void setup()
{
  // put your setup code here, to run once:

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:

  digitalWrite(LED_BUILTIN, HIGH); // turn the led off
  delay(1000);                     // wait for 1 sec
  digitalWrite(LED_BUILTIN, LOW);  // turn the led on
  delay(1000);                     // wait for 1 sec
}