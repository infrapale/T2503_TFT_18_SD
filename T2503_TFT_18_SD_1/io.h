#ifndef __IO_H__ 
#define __IO_H__
#include <Arduino.h>
#define BOARD_PICO_TFT_4KEYS

#define PIN_WIRE_SDA        (12u)
#define PIN_WIRE_SCL        (13u)

#define PIN_TFT_CS          (17u)
#define PIN_TFT_RST         (7u)  
#define PIN_TFT_DC          (10u)
#define PIN_TFT_LED         (14u) 

#define PIN_LOGGER_SD_CS    (6u) 

#define PIN_KEY1		        (13u)
#define PIN_KEY2		        (12u)
#define PIN_KEY3		        (11u)
#define PIN_LDR             (26u)
#define PIN_PIR             (27u)
#define PIN_


void io_initialize(void);

uint16_t io_read_ldr(void);

bool io_read_pir(void);

#endif
