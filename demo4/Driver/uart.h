/*
 * uart.h
 *
 *  Created on: 2026年1月19日
 *      Author: 33491
 */

#ifndef DRIVER_UART_H_
#define DRIVER_UART_H_

void Usart3_Init();//发送语音模块，接收刷卡模块//一开始这样考虑，为什么不行呢？因为语音模块波特率9600 刷卡模块115200
void Usart2_Init();//接收刷卡模块

#endif /* DRIVER_UART_H_ */
