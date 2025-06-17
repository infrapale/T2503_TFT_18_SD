#include "main.h"
#include "atask.h"
#include "menu.h"


extern lora_st lora;

void lora_task(void);

//                                          123456789012345   ival  next  state  prev  cntr flag  call backup
atask_st lora_task_handle             =   {"LoRa Task      ", 1000,   0,     0,  255,    0,   1,  lora_task };


void lora_initialize(void)
{
    atask_add_new(&lora_task_handle);
}

void lora_send_msg(void)
{
   char buff[40];
   memset(buff,0x00,40);
   sprintf(buff,"<RSND;%d;%d;%d;%d;%d;%d;%d>",
      lora.my_addr,
      lora.base_addr,
      1,
      lora.power,
      lora.spreading_factor,
      lora.my_counter,
      lora.base_counter);
   Serial.println(buff);   
   Serial1.println(buff);   
   //Serial1.println("ABC");
}

void lora_receive_msg(void)
{
   String Str;
    if (Serial1.available())
    {
        Serial.println("Message from base is available");
        Str = Serial1.readStringUntil('>');
        if (Str.length()> 0)
        {
            //rx_msg.avail = true;
            //rx_msg.str.remove(rx_msg.str.length()-1);
            //Serial1.println(rx_msg.str);
            //status = STATUS_AVAILABLE;
            Serial.println(Str);
        }
    } 

}

void lora_task(void)
{
   if(++lora.ival_cntr >= lora.interval)
   {
      lora.my_counter++;
      lora.ival_cntr = 0;
      Serial.printf("Cntr = %d\n", lora.ival_cntr);
      menu_update();
      lora_send_msg();
      delay(100);
      lora_receive_msg();
   }
}
