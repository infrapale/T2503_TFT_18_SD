/**
https://randomnerdtutorials.com/guide-to-1-8-tft-display-with-arduino/

**/

#include <SPI.h>
#include "main.h"
#include "tftx.h"
#include "atask.h"
#include "dashboard.h"
#include "logger.h"
#include "menu.h"

void print_debug_task(void)
{
  atask_print_status(true);
  Serial.printf("LDR= %d  PIR = %d\n", io_read_ldr(), io_read_pir());
}

//                                  123456789012345   ival  next  state  prev  cntr flag  call backup
atask_st debug_task_handle    =   {"Debug Task     ", 5000,    0,     0,  255,    0,  1,  print_debug_task };


void setup(void) {
  Serial.begin(9600); 
  while(!Serial){}
  delay(2000);
  Serial.print("T2503_TFT_18_SD"); Serial.print(" Compiled: ");
  Serial.print(__DATE__); Serial.print(" ");
  Serial.print(__TIME__); Serial.println();
  io_initialize();
  atask_initialize();
  atask_add_new(&debug_task_handle);
  logger_initialize(); 
  tftx_initialize();
  dashboard_initialize();
  dashboard_start_task();
  menu_initialize();
  tftx_update_boxes();
}

void loop() {
  atask_run();
  /*
  tft.invertDisplay(true);
  delay(500);
  tft.invertDisplay(false);
  delay(500);
  */
}


