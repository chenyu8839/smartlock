/*
 * uart.h
 *
 *  Created on: 2026年1月19日
 *      Author: 33491
 */

#ifndef DRIVER_UART_H_
#define DRIVER_UART_H_

void Usart3_Init();//发送语音模块，接收刷卡模块//一开始这样考虑，为什么不行呢？因为语音模块波特率9600 刷卡模块115200
void Usart2_Init();//接收刷卡模块 115200
void Uart7_Init();//指纹57600
void uart7_send_string(u8* string,u8 len);
void Uart6_Init();
void uart6_send_string(u8* string,u8 len);
void usart1_send_string(u8* string,u8 len);

#endif /* DRIVER_UART_H_ */
