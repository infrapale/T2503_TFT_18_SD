/**

https://randomnerdtutorials.com/guide-to-1-8-tft-display-with-arduino/


**/

#include <SPI.h>
#include "tftx.h"


void setup(void) {
  Serial.begin(9600); 
  while(!Serial){}
  delay(2000);
  Serial.print("T2503_TFT_18_SD"); Serial.print(" Compiled: ");
  Serial.print(__DATE__); Serial.print(" ");
  Serial.print(__TIME__); Serial.println();
  tftx_initialize();
  tftx_update_boxes();
}

void loop() {
  /*
  tft.invertDisplay(true);
  delay(500);
  tft.invertDisplay(false);
  delay(500);
  */
}


