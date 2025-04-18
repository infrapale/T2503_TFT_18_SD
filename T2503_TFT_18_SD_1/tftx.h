#ifndef __TFTX_H__
#define __TFTX_H__

#include <Arduino.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     17
#define TFT_RST    14  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     13

//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
// Option 2: use any pins but a little slower!
#define TFT_SCLK 13   // set these to be whatever pins you like!
#define TFT_MOSI 11   // set these to be whatever pins you like!
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

typedef enum
{
  FONT_STANDARD = 0,
  FONT_SANS_9,
  FONT_SANS_12,
  FONT_SANS_18,
  FONT_SANS_24,
  FONT_SANS_BOLD_9,
  FONT_SANS_BOLD_12,
  FONT_SANS_BOLD_18,
  FONT_SANS_BOLD_24,
  FONT_NBR_OF
} font_et;

typedef enum 
{
  BOX_FULL_SCREEN = 0,
  BOX_MENU_1,
  BOX_MENU_2,
  BOX_MENU_3,
  BOX_TOP,
  BOX_MID,
  BOX_LOW,
  BOX_NBR_OF
} box_et;

typedef struct
{
  bool    update;
  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
  uint16_t frame;
  uint16_t fill; 
  char text[80];
  font_et font;
  uint16_t txt_color;
  bool    txt_wrap;
} box_st;

void tftx_initialize(void);

void tftx_draw_box(box_st *boxp);

void tftx_update_boxes(void);

#endif