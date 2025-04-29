#include <Arduino.h>
#include "main.h"
#include "io.h"

void io_initialize(void)
{
  analogReadResolution(12);
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_LDR, INPUT);
  pinMode(PIN_KEY1, INPUT_PULLUP);
  pinMode(PIN_KEY2, INPUT_PULLUP);
  pinMode(PIN_KEY3, INPUT_PULLUP);

}

uint16_t io_read_ldr(void)
{
  return analogRead(PIN_LDR);
}

bool io_read_pir(void)
{
  return (digitalRead(PIN_PIR) == HIGH);
}