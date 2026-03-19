/*
 * audio.c
 *
 *  Created on: 2026年1月19日
 *      Author: 33491
 */

#include "debug.h"
#include "audio.h"
#include "uart.h"

void audio_init()
{
    Usart3_Init();
}

void audio_play(u8 num)
{
   u8 string[]={0x7e,0x05,0x41,0x00,num,0x05^0x41^0x00^num,0xef};
   u8 i;
   for(i=0;i<7;i++)
   {
       USART_SendData(USART3,string[i]);
       while(  USART_GetFlagStatus(USART3, USART_FLAG_TC)==0 );//等待发送完成
   }
}


void audio_volume(u8 volume)
{
    u8 string[]={0x7e,0x04,0x31,volume,0x04^0x31^volume,0xef};
    u8 i;
   for(i=0;i<6;i++)
   {
       USART_SendData(USART3,string[i]);
       while(  USART_GetFlagStatus(USART3, USART_FLAG_TC)==0 );//等待发送完成
   }
}
