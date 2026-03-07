/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 USART Print debugging routine:
 USART1_Tx(PA9).
 This example demonstrates using USART1(PA9) as a print debug port output.

*/

#include "debug.h"

unsigned char led_flag;
unsigned int time_1000ms;

void TIM3_IRQHandler() __attribute__((interrupt("WCH-Interrupt-fast")));

void TIM3_Init(u16 period, u16 prescaler)
{
    // 定义定时器基本配置的结构体变量
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    // 定义中断向量控制器(NVIC)配置的结构体变量
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    /*----------- 步骤 1: 使能硬件时钟 -----------*/
    // 使能 TIM3 定时器的外设时钟。TIM3 挂载在 APB1 总线上。
    // 就像要用电器必须先插上电一样，要用定时器必须先给它“通电”。
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /*----------- 步骤 2: 配置定时器核心参数 -----------*/
    // 设置自动重装载值。计数器会从 0 数到这个值，然后产生溢出事件。
    TIM_TimeBaseStructure.TIM_Period = period;
    // 设置预分频器值。它会将输入的时钟频率进行分频，作为计数器的计数频率。
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
    // 设置时钟分频因子。这个参数主要用于死区和采样，这里设为0（不分频）即可。
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; // 等同于 TIM_CKD_DIV1
    // 设置计数模式为向上计数。计数器从 0 递增到 'period' 值。
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    // 调用库函数，将上面配置好的参数写入到 TIM3 的寄存器中。
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /*----------- 步骤 3: 配置并使能中断 -----------*/
    // 使能 TIM3 的“更新中断”和“触发中断”。
    // 更新中断是计数器溢出时产生的，是我们最常用的。
    // 触发中断通常用于和其他外设（如ADC）联动，这里可能不是必须的。
    TIM_ITConfig(TIM3, TIM_IT_Update | TIM_IT_Trigger, ENABLE);

    // 设置 TIM3 在中断控制器(NVIC)中的配置。
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; // 指定中断通道为 TIM3。
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能这个中断通道。
    // 调用库函数，初始化 NVIC。
    NVIC_Init(&NVIC_InitStructure);

    /*----------- 步骤 4: 启动定时器 -----------*/
    // 正式启动 TIM3 计数器。调用它之后，定时器就开始工作了。
    TIM_Cmd(TIM3, ENABLE);
}

/* 中断服务函数 */
void TIM3_IRQHandler()
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update)!= RESET)
    {
        if(++time_1000ms == 1000)
        {
            time_1000ms = 0;
            led_flag ^= 1;
            if(led_flag)
            {
                GPIO_SetBits(GPIOC, GPIO_Pin_2);
            }
                else
            {

                GPIO_ResetBits(GPIOC, GPIO_Pin_2);
            }

        }
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();



	Delay_Init();
	USART_Printf_Init(115200);	
	printf("i love myself\r\n");

	GPIO_InitTypeDef GPIO_InitStructure = {0};

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure);

	TIM3_Init(999,95);

	while(1)
    {
//	    GPIO_SetBits(GPIOC, GPIO_Pin_2);
//	    Delay_Ms(500);
//	    GPIO_ResetBits(GPIOC, GPIO_Pin_2);
//	    Delay_Ms(500);

	}
}

