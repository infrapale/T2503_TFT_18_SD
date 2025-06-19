#include "main.h"
#include "atask.h"
#include "menu.h"


extern lora_st lora;

void lora_task(void);

//                                          123456789012345   ival  next  state  prev  cntr flag  call backup
atask_st lora_th                      =   {"LoRa Task      ", 100,   0,     0,  255,    0,   1,  lora_task };


void lora_initialize(void)
{
    atask_add_new(&lora_th);
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

bool lora_receive_msg(void)
{
   String Str;
   bool msg_received = false;
   msg_status_et status = STATUS_UNDEFINED;
   bool do_continue = true;

   if (Serial1.available())
   {
         Serial.println("Message from base is available");
         Str = Serial1.readStringUntil('\n');
         if (Str.length()> 0)
         {
            uint8_t len;
            Str->trim();
            Serial.print(*Str);
            len = Str->length();
            Serial.println(len);
            if ((Str->charAt(0) != '<') || 
               (Str->charAt(len-1) != '>'))  do_continue = false;
            if (do_continue)
            {   
               status = STATUS_OK_FOR_ME;
               #ifdef DEBUG_PRINT
               Serial.print("Buffer frame is OK\n");
               #endif
            }
            else status = STATUS_INCORRECT_FRAME;


            msg_received = true;
            //rx_msg.avail = true;
            //rx_msg.str.remove(rx_msg.str.length()-1);
            //Serial1.println(rx_msg.str);
            //status = STATUS_AVAILABLE;
            Serial.println(Str);
         }
   } 
   return msg_received;
}

void lora_task(void)
{
   static uint32_t timeout_ms;

   switch(lora_th.state)
   {
      case 0:
         lora_th.state = 10;
         break;
      case 10:
         lora_th.state = 11;
         timeout_ms = millis() + 5000;
         break;
      case 11:
         if (millis() > timeout_ms) lora_th.state = 20;
         break;
      case 20:
         lora.my_counter++;
         Serial.printf("Cntr = %d\n", lora.my_counter);
         menu_update();
         lora_send_msg();
         lora_th.state = 30;
         timeout_ms = millis() + 2000;
         break;
      case 30:
         if (lora_receive_msg())
         {
            lora_th.state = 100;
         }
         else
         {
            if (millis() > timeout_ms) lora_th.state = 40;
         }
         break;
      case 40:
         Serial.println("No reply received");
         lora_th.state = 100;
         break;
      case 50:
         lora_th.state = 100;
         break;
      case 100:
         lora_th.state = 10;
         break;
      
   }
   // if(++lora.ival_cntr >= lora.interval)
   // {
   //    lora.my_counter++;
   //    lora.ival_cntr = 0;
   //    Serial.printf("Cntr = %d\n", lora.ival_cntr);
   //    menu_update();
   //    lora_send_msg();
   //    //delay(100);
   //    lora_receive_msg();
   // }
   
}
