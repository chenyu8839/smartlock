/*
 * timer.c
 *
 *  Created on: 2025年11月25日
 *      Author: 33491
 */
#include "debug.h"

// 定义舵机的PWM周期和占空比
#define PWM_PERIOD 20000  // PWM周期为20ms (20000us)
#define PWM_MIN 500       // 最小脉宽为0.5ms (500us)
#define PWM_MAX 2500      // 最大脉宽为2.5ms (2500us)

// 初始化TIM2以生成PWM信号
void TIM2_PWM_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // 使能TIM2时钟

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 配置TIM2的基本参数
    TIM_TimeBaseStructure.TIM_Period = PWM_PERIOD - 1;  // 设置自动重装值
    TIM_TimeBaseStructure.TIM_Prescaler = 96 - 1;       // 设置预分频器，96MHz/96 = 1MHz (1us)
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 配置TIM2的PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = PWM_MIN;  // 初始占空比为最小值
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    // 配置TIM2通道1
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);  // 使用TIM2的通道1
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  // 使能TIM2通道1的预装载寄存器


    TIM_ARRPreloadConfig(TIM2, ENABLE);               // 使能TIM2的自动重装寄存器预装载

    TIM_Cmd(TIM2, ENABLE);  // 使能TIM2


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // 使能GPIOA时钟

    GPIO_InitTypeDef GPIO_InitStructure;

    // 配置PA0为复用推挽输出（TIM2通道1）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void lock(u8 enable)
{
    if(enable)
    {
        TIM_SetCompare1(TIM2,500);
    }
    else
    {
        TIM_SetCompare1(TIM2,1500);
    }

}

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
