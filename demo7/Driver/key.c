/*
 * key.c
 *
 *  Created on: 2025年11月25日
 *      Author: 33491
 */


#include "debug.h"
#include "key.h"
/*
 * OUT PP
 * R4 PD11
 * R3 PD9
 * R2 PE15
 * R1 PE13
 *
 * IPU
 * C1 PE11
 * C2 PE9
 * C3 PE7
 * C4 PC5
 */
void key_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_9|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

}

u8 key_read()
{
    u8 temp = 0;

    /* * 修正后的逻辑说明：
     * 之前的现象是 S1->16, S16->1，说明引脚完全反了。
     * 因此，我们要把之前的 R4(PD11) 当作第一行来扫，
     * 把之前的 C4(PC5) 当作第一列来读。
     */

    // ======================================================
    // 1. 扫描物理上的第一行 (对应之前的 R4: PD11) -> 目标键值 1 - 4
    // ======================================================
    // 动作：拉低 PD11，拉高其他行 (PD9, PE15, PE13)
    GPIO_SetBits(GPIOD, GPIO_Pin_9);
    GPIO_SetBits(GPIOE, GPIO_Pin_15 | GPIO_Pin_13);
    GPIO_ResetBits(GPIOD, GPIO_Pin_11); // 拉低 PD11 (现在的 Row 1)

    // 检测列 (顺序也要反过来：先测 PC5，最后测 PE11)
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0) temp = 1;  // 之前的 C4 (PC5) 现在是 Col 1
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7) == 0) temp = 2;  // 之前的 C3 (PE7) 现在是 Col 2
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9) == 0) temp = 3;  // 之前的 C2 (PE9) 现在是 Col 3
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11)== 0) temp = 4;  // 之前的 C1 (PE11) 现在是 Col 4

    // ======================================================
    // 2. 扫描物理上的第二行 (对应之前的 R3: PD9) -> 目标键值 5 - 8
    // ======================================================
    // 动作：拉低 PD9，拉高其他行
    GPIO_SetBits(GPIOD, GPIO_Pin_11);
    GPIO_SetBits(GPIOE, GPIO_Pin_15 | GPIO_Pin_13);
    GPIO_ResetBits(GPIOD, GPIO_Pin_9); // 拉低 PD9 (现在的 Row 2)

    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0) temp = 5;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7) == 0) temp = 6;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9) == 0) temp = 7;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11)== 0) temp = 8;

    // ======================================================
    // 3. 扫描物理上的第三行 (对应之前的 R2: PE15) -> 目标键值 9 - 12
    // ======================================================
    // 动作：拉低 PE15，拉高其他行
    GPIO_SetBits(GPIOD, GPIO_Pin_11 | GPIO_Pin_9);
    GPIO_SetBits(GPIOE, GPIO_Pin_13);
    GPIO_ResetBits(GPIOE, GPIO_Pin_15); // 拉低 PE15 (现在的 Row 3)

    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0) temp = 9;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7) == 0) temp = 10;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9) == 0) temp = 11;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11)== 0) temp = 12;

    // ======================================================
    // 4. 扫描物理上的第四行 (对应之前的 R1: PE13) -> 目标键值 13 - 16
    // ======================================================
    // 动作：拉低 PE13，拉高其他行
    GPIO_SetBits(GPIOD, GPIO_Pin_11 | GPIO_Pin_9);
    GPIO_SetBits(GPIOE, GPIO_Pin_15);
    GPIO_ResetBits(GPIOE, GPIO_Pin_13); // 拉低 PE13 (现在的 Row 4)

    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0) temp = 13;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7) == 0) temp = 14;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9) == 0) temp = 15;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11)== 0) temp = 16;

    return temp;
}


