/*
 * timer.h
 *
 *  Created on: 2025Äê11ÔÂ25ÈÕ
 *      Author: 33491
 */

#ifndef DRIVER_TIMER_H_
#define DRIVER_TIMER_H_

void TIM2_PWM_Init(void);
void lock(u8 enable);
void TIM3_Init(u16 period, u16 prescaler);

#endif /* DRIVER_TIMER_H_ */
