// https://learn.adafruit.com/adafruit-gfx-graphics-library

#include "tftx.h"

#include <Arduino.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>	
#include <Fonts/FreeMonoOblique9pt7b.h>
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

#define TFT_WIDTH    160
#define TFT_HEIGHT   128
#define MENU_HEIGHT  16
#define TOP_BOX_HEIGHT 24
#define MID_BOX_HEIGHT 50
#define LOW_BOX_HEIGHT 36

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

int16_t font_voffs[FONT_NBR_OF] =
{
  [FONT_STANDARD] = 5,
  [FONT_SANS_9] = 12,
  [FONT_SANS_12] = 12,
  [FONT_SANS_18] = 28,
  [FONT_SANS_24] = 40,
  [FONT_SANS_BOLD_9] = 12,
  [FONT_SANS_BOLD_12] = 12,
  [FONT_SANS_BOLD_18] = 18,
  [FONT_SANS_BOLD_24] = 24,
};
box_st box[BOX_NBR_OF];

void tftx_initialize(void)
{
  for (uint8_t i = 0; i <= BOX_NBR_OF; i++)
  {
    box[i].update = true;
  }

  for (uint8_t i = BOX_MENU_1; i <= BOX_MENU_3; i++)
  {
    box[i].update     = true;
    box[i].x          = (i-1) * TFT_WIDTH/3;
    box[i].y          = TFT_HEIGHT - MENU_HEIGHT;
    box[i].w          = TFT_WIDTH/3;
    box[i].h          = MENU_HEIGHT;
    box[i].frame      = ST7735_YELLOW;
    box[i].fill       = ST7735_BLUE;
    box[i].font       = FONT_STANDARD;
    box[i].txt_color  = ST7735_CYAN;
  }
  strcpy(box[BOX_MENU_1].text, "Powr");
  strcpy(box[BOX_MENU_2].text, "Opt");
  strcpy(box[BOX_MENU_3].text, "Test");

  box[BOX_TOP].update = true;
  box[BOX_TOP].x = 0;
  box[BOX_TOP].y = 0;
  box[BOX_TOP].w = TFT_WIDTH;
  box[BOX_TOP].h = TOP_BOX_HEIGHT;
  box[BOX_TOP].frame = ST7735_WHITE;
  box[BOX_TOP].fill =  ST7735_BLACK;
  box[BOX_TOP].font = FONT_SANS_12;
  box[BOX_TOP].txt_color = ST7735_WHITE;
  strcpy(box[BOX_TOP].text, "Sending");

  box[BOX_MID].update = true;
  box[BOX_MID].x = 0;
  box[BOX_MID].y = box[BOX_TOP].y + box[BOX_TOP].h + 1;
  box[BOX_MID].w = TFT_WIDTH;
  box[BOX_MID].h = MID_BOX_HEIGHT;
  box[BOX_MID].frame = ST7735_YELLOW;
  box[BOX_MID].fill =  ST7735_BLACK;
  box[BOX_MID].font = FONT_SANS_24;
  box[BOX_MID].txt_color = ST7735_YELLOW;
  strcpy(box[BOX_MID].text, "#1234");

  box[BOX_LOW].update = true;
  box[BOX_LOW].x = 0;
  box[BOX_LOW].y = box[BOX_MID].y + box[BOX_MID].h + 1;
  box[BOX_LOW].w = TFT_WIDTH;
  box[BOX_LOW].h = LOW_BOX_HEIGHT;
  box[BOX_LOW].frame = ST7735_CYAN;
  box[BOX_LOW].fill =  ST7735_RED;
  box[BOX_LOW].font = FONT_SANS_18;
  box[BOX_LOW].txt_color = ST7735_BLACK;
  strcpy(box[BOX_LOW].text, "Err 42");



  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  Serial.println("Initialized");
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(3);
  
}

void tftx_set_font(font_et font)
{
    switch(font)
    {
      case FONT_STANDARD:
        tft.setFont();
        break;
      case FONT_SANS_9:
        tft.setFont(&FreeSans9pt7b);
        break;
      case FONT_SANS_12:
        tft.setFont(&FreeSans12pt7b);
        break;
      case FONT_SANS_18:
        tft.setFont(&FreeSans18pt7b);
        break;
      case FONT_SANS_24:
        tft.setFont(&FreeSans24pt7b);
        break;
      case FONT_SANS_BOLD_9:
        tft.setFont(&FreeSansBold9pt7b);
        break;
      case FONT_SANS_BOLD_12:
        tft.setFont(&FreeSansBold12pt7b);
        break;
      case FONT_SANS_BOLD_18:
        tft.setFont(&FreeSansBold18pt7b);
        break;
      case FONT_SANS_BOLD_24:
        tft.setFont(&FreeSansBold24pt7b);
        break;
      default:
        tft.setFont(&FreeSansBold9pt7b);
        break;
    }
}

void tftx_draw_box(box_st *boxp)
{
    tft.fillRect(boxp->x, boxp->y, boxp->w, boxp->h, boxp->fill);
    tft.drawRect(boxp->x, boxp->y, boxp->w, boxp->h, boxp->frame);
}

void tftx_update_boxes(void)
{
  for (uint8_t i = 0; i < BOX_NBR_OF; i++)
  {
      if (box[i].update)
      {
        tftx_draw_box(&box[i]);
        tft.setTextWrap(box[i].txt_wrap);
        tft.setCursor(box[i].x + 2, box[i].y + font_voffs[box[i].font]);
        //tft.setCursor(menu_box[i].x + 2, menu_box[i].y + 18);
        tftx_set_font(box[i].font);
        tft.setTextColor(box[i].txt_color);
        tft.print(box[i].text);
      }
  }
  //tftx_draw_box(&box_test);
}