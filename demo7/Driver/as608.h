/*
 * as608.h
 *
 *  Created on: 2026Äê3ÔÂ1ÈÕ
 *      Author: 33491
 */

#ifndef DRIVER_AS608_H_
#define DRIVER_AS608_H_

void as608_init();
void PS_GetImage();
void PS_GenChar(u8 buffer);
void PS_RegModel();
void PS_StoreChar(u8 addr);
void PS_Search();
void PS_Empty();
void As608_Gpio_Init();

#endif /* DRIVER_AS608_H_ */
