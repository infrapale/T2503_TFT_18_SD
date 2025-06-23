#include "main.h"
#include "atask.h"
#include "menu.h"
#include "lora.h"
#include "tftx.h"
#include "dashboard.h"


extern lora_st lora;

void lora_task(void);

//                                          123456789012345   ival  next  state  prev  cntr flag  call backup
atask_st lora_th                      =   {"LoRa Task      ", 100,   0,     0,  255,    0,   1,  lora_task };

cmd_st commands[CMD_NBR_OF] = 
{
  [CMD_UNDEFINED]       = {"UNDF"},
  [CMD_RADIO_SEND]      = {"RSND"},
  [CMD_RADIO_RECEIVE]   = {"RREC"},
  [CMD_SET_POWER]       = {"SPWR"},
  [CMD_RADIO_RESET]     = {"RRST"},
  [CMD_SET_SF]          = {"S_SF"},
  [CMD_RADIO_REPLY]     = {"RREP"},
  [CMD_GET_RSSI]        = {"RSSI"},
};

msg_st      rply_msg;
msg_data_st msg_values  = {0};


void lora_initialize(void)
{
    atask_add_new(&lora_th);
}

void lora_send_msg(void)
{
   char buff[40];
   memset(buff,0x00,40);
   sprintf(buff,"<RSND;%d;%d;%d;%d;%d;%d;%d;%d>",
      lora.my_addr,
      lora.base_addr,
      1,
      lora.power,
      0,
      lora.spreading_factor,
      lora.my_counter,
      lora.base_counter);
   Serial.println(buff);   
   Serial1.println(buff);   
   //Serial1.println("ABC");
}


void parser_rd_msg_values(msg_data_st *msg_data, String *StrP)
{
    uint8_t indx1 = 1;
    uint8_t indx2 = StrP->indexOf(';');
    bool    do_continue = true;
    uint8_t end_pos = StrP->length();

    StrP->substring(indx1,indx2).toCharArray( msg_data->tag , CMD_TAG_LEN);
    msg_data->tag_indx = CMD_NBR_OF;
    for (uint8_t i = 0; i < CMD_NBR_OF; i++)
    {
        if(strcmp(msg_data->tag, commands[i].tag) == 0) 
        {
            msg_data->tag_indx = (cmd_et)i;
            break;
        }
    }

    indx1 = indx2+1;
    msg_data->nbr_values = 0;
    if((indx1 < end_pos) && (msg_data->tag_indx < CMD_NBR_OF)) do_continue = true;
    else do_continue = false;

    while(do_continue)
    {
        indx2 = StrP->indexOf(';',indx1+1);
        msg_data->value[msg_data->nbr_values] = StrP->substring(indx1,indx2).toInt();
        indx1 = indx2+1;
        msg_data->nbr_values++;
        if ((indx2 >= end_pos) || (msg_data->nbr_values >= CMD_MAX_VALUES)) do_continue = false;
    }
}

void parser_build_msg_from_fields(char *msg, msg_st *msg_data)
{
    sprintf(msg,"<%s;%d;%d;%d;%d;%d;%d;%d;%d>",
        msg_data->field.cmnd,
        msg_data->field.from,
        msg_data->field.target,
        msg_data->field.radio,
        msg_data->field.power,
        msg_data->field.rssi,
        msg_data->field.sf,
        msg_data->field.remote_nbr,
        msg_data->field.base_nbr
    );
}

msg_status_et lora_check_frame(String *Str)
{
   msg_status_et status = STATUS_UNDEFINED;
   //rfm_send_msg_st *rx_msg = &send_msg; 
   bool do_continue = true;
   Str->trim();
   uint8_t len = Str->length();
   // Serial.print(*Str); Serial.println(len);
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
   return status;
}



bool lora_receive_msg(void)
{
      String Str;
      bool msg_received = false;
      msg_status_et rply_status = STATUS_UNDEFINED;
      

      if (Serial1.available())
      {
            Serial.println("Message from base is available");
            Str = Serial1.readStringUntil('\n');
            if (Str.length()> 0)
            {
               rply_status = lora_check_frame(&Str);
               if (rply_status == STATUS_OK_FOR_ME) 
               {
                  parser_rd_msg_values(&msg_values, &Str);
                  rply_msg.field.from        = msg_values.value[0];
                  rply_msg.field.target      = msg_values.value[1];
                  rply_msg.field.radio       = msg_values.value[2];
                  rply_msg.field.power       = msg_values.value[3];
                  rply_msg.field.rssi        = msg_values.value[4];
                  rply_msg.field.sf          = msg_values.value[5];
                  rply_msg.field.remote_nbr  = msg_values.value[6];
                  rply_msg.field.base_nbr    = msg_values.value[7];

                  msg_received = true;
                  Serial.println(Str);
               }
                
            }
      } 
      return msg_received;
}

void lora_request_rssi(void)
{
   Serial1.println("<RSSI;0>");   
}

int lora_get_rssi(void)
{
      String Str;
      int rssi = 0;

      if (Serial1.available())
      {
            Str = Serial1.readStringUntil('\n');
            if (Str.length()> 0)
            {
               msg_status_et rply_status = lora_check_frame(&Str);
               if (rply_status == STATUS_OK_FOR_ME) 
               {
                  parser_rd_msg_values(&msg_values, &Str);
                  rssi         = msg_values.value[0];
                  Serial.println(Str);
               }
            }
      } 
      return rssi;
}

void lora_update_dashboard(void)
{
   char txt[40];

   sprintf(txt,"R# %d B#%d RSSI %d",
   rply_msg.field.remote_nbr,
   rply_msg.field.base_nbr,
   rply_msg.field.rssi);
   dashboard_set_row_text(2, txt);

   sprintf(txt,"B:%d R:%d", rply_msg.field.rssi, lora.my_rssi);
   dashboard_set_mid_text(txt);
   if(lora.my_rssi < 0)
   {
      dashboard_set_mid_color(COLOR_YELLOW);
      dashboard_set_mid_font(FONT_SANS_12);
   }   
   else
   {
      dashboard_set_mid_text((char*)"Hetkinen");
      dashboard_set_mid_color(COLOR_RED);
      dashboard_set_mid_font(FONT_SANS_18);
   } 
   menu_update();

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
         lora_send_msg();
         lora_th.state = 30;
         timeout_ms = millis() + 2000;
         break;
      case 30:
         if (lora_receive_msg())
         {
            lora_th.state = 50;
         }
         else
         {
            if (millis() > timeout_ms) lora_th.state = 40;
         }
         break;
      case 40:
         Serial.println("No reply received");
         lora.my_rssi = 0;
         lora_th.state = 100;
         break;
      case 50:
         lora_request_rssi();
         timeout_ms = millis() + 500;
         lora_th.state = 51;
         break;
      case 51:
         lora.my_rssi = lora_get_rssi();
         Serial.printf("lora.my_rssi=%d\n",lora.my_rssi);
         if((lora.my_rssi < 0) || (millis() > timeout_ms)) lora_th.state = 100;
         break;
      case 100:
         lora_update_dashboard();
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
